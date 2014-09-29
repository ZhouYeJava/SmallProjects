########################
####Checkee Analysis####
########################

####Zhou Ye####
####09/22/2014####

rm(list=ls())
setwd("/Users/zhouye/Dropbox/Checkee_Analysis/")
suppressMessages(library(XML))
suppressMessages(library(zoo))
suppressMessages(library(ggplot2))
suppressMessages(library(scales))

####Read Table From Website####
checkee_info <- list()
iter <- 0
for (year in 2009:2013) {
    for (month in 1:12) {
        date <- as.Date(as.yearmon(paste(year, month, sep="-")))
        url <- paste0("http://www.checkee.info/main.php?dispdate=", substring(date, 1, 7))
        data <- readHTMLTable(url)[[8]] #read table from checkee
        names(data) <- sub(" ", "_", names(data)) #remove space in column names
        names(data) <- sub("\\(s\\)", "", names(data)) #remove bracket in "waiting day(s)"
        data$Update <- NULL
        data$Details <- NULL
        data$Year <- year
        data$Month <- month
        iter <- iter+1
        print(iter)
        checkee_info[[iter]] <- data
    }
}
checkee_data <- do.call("rbind", checkee_info)
checkee_data$Waiting_Day <- as.numeric(as.character(checkee_data$Waiting_Day))
clear_data <- subset(checkee_data, Status=="Clear")
save(clear_data, file="clear_2009_2013.RData")

####Month####
ggplot(clear_data, aes(x=factor(Month), y=Waiting_Day))+
    geom_boxplot(outlier.shape=NA)+
    scale_y_continuous(limits=quantile(clear_data$Waiting_Day, c(0.1, 0.9)))+
    labs(x="Month", y="Waiting Days")

####Visa Type####
ggplot(clear_data, aes(x=factor(Visa_Type), y=Waiting_Day))+
    geom_boxplot(outlier.shape=NA)+
    scale_y_continuous(limits=quantile(clear_data$Waiting_Day, c(0.1, 0.9)))+
    labs(x="Visa Type", y="Waiting Days")

####Visa Entry####
entry_data <- subset(clear_data, Visa_Entry!="")
ggplot(entry_data, aes(x=factor(Visa_Entry), y=Waiting_Day))+
    geom_boxplot(outlier.shape=NA)+
    scale_y_continuous(limits=quantile(clear_data$Waiting_Day, c(0.1, 0.9)))+
    labs(x="Visa Entry", y="Waiting Days")

####City####
city_data <- within(clear_data, {
    City[!City%in%c("ShangHai", "ShenYang", "BeiJing", "GuangZhou", "ChengDu")] <- "Others"
    })
ggplot(city_data, aes(x=factor(City), y=Waiting_Day))+
    geom_boxplot(outlier.shape=NA)+
    scale_y_continuous(limits=quantile(clear_data$Waiting_Day, c(0.1, 0.9)))+
    labs(x="City", y="Waiting Days")

####Month/Year####
monthly_waiting <- aggregate(Waiting_Day~Month+Year, data=clear_data, median)
ggplot(monthly_waiting, aes(x=factor(Month), y=Waiting_Day, group=factor(Year), colour=factor(Year)))+
    geom_line()+labs(x="Month", y="Waiting Days")+
    scale_colour_discrete(name="Year")

####F1/Month/Year####
f1_data <- subset(clear_data, Visa_Type=="F1")
f1_waiting <- aggregate(Waiting_Day~Month+Year, data=f1_data, median)
ggplot(f1_waiting, aes(x=factor(Month), y=Waiting_Day, group=factor(Year), colour=factor(Year)))+
    geom_line()+labs(x="Month", y="Waiting Days")+
    scale_colour_discrete(name="Year")

####F1/CS/Month/Year####
cs_data <- subset(f1_data, Major%in%c("CS", "Computer Science", "cs", "C.S.", "Computer Science(CS)", 
                                         "computer science", "CompSci"))
cs_waiting <- aggregate(Waiting_Day~Month+Year, data=cs_data, median)
ggplot(cs_waiting, aes(x=factor(Month), y=Waiting_Day, group=factor(Year), colour=factor(Year)))+
    geom_line()+labs(x="Month", y="Waiting Days")+
    scale_colour_discrete(name="Year")


