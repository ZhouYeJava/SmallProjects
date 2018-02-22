smart_diff_max <- function(seq) {
  seq_reduct <- seq[-length(seq)]-seq[-1]
  seq_divide <- seq[-length(seq)]
  return(get_max_sum(seq_reduct, seq_divide, 1, length(seq_reduct)))
}

get_max_sum <- function(seq1, seq2, left, right) {
  if (left>right) {
    return(0)
  }
  if (left==right) {
    return(seq1[left]/seq2[left])
  }
  middle <- as.integer((left+right)/2)
  left_max <- seq1[middle]/seq2[middle]
  left_sum <- seq1[middle]/seq2[middle]
  left_index <- middle
  if (middle>left) {
    for (i in (middle-1):left) {
      left_sum <- (left_sum*seq2[i+1]+seq1[i])/seq2[i]
      if (left_sum > left_max) {
        left_max <- left_sum
        left_index <- i
      }
    }
  }
  right_max <- seq1[middle+1]/seq2[middle+1]
  right_sum <- 0
  for (i in (middle+1):right) {
    right_sum = (right_sum*seq2[middle+1]+seq1[i])/seq2[middle+1]  
    if (right_sum > right_max) {
      right_max <- right_sum
    }
  }
  return(max((left_max*seq2[left_index]+right_max*seq2[middle+1])/seq2[left_index], get_max_sum(seq1, seq2, left, middle), get_max_sum(seq1, seq2, middle+1, right)))
}

stupid_diff_max <- function(seq) {
  dist <- c()
  for (i in 1:length(seq)) {
    dist[i] <- (seq[i]-min(seq[(i+1):length(seq)]))/seq[i]
  }
  return(max(dist[!is.na(dist)]))
}

a <- runif(10000)
t1 <- Sys.time()
print(smart_diff_max(a))
t2<- Sys.time()
print(stupid_diff_max(a))
t3 <- Sys.time()
print(t2-t1)
print(t3-t2)