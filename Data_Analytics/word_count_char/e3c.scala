
var linesRDD = sc.textFile("odyssey.txt")

var wordsRDD = linesRDD.flatMap(line=>line.split(""))

var wordsvRdd = wordsRDD.map(char=>(char,1))

var wordCounts = wordsvRdd.reduceByKey(_ + _ )

wordCounts.saveAsTextFile("e3c")
