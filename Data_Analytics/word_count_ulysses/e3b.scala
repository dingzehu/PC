

var linesRDD = sc.textFile("odyssey.txt")

var wordsRDD = linesRDD.flatMap(_.split(" "))

var wordsvRdd = wordsRDD.map((_, 1))

var wordCounts = wordsvRdd.reduceByKey(_ + _ )

wordCounts.collect.length

wordCounts.saveAsTextFile("e3b")
