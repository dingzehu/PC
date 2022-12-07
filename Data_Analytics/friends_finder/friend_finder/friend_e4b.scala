def Mapper(line: String) = {
    val words = line.split(" ")
    val key = words(0)
    val fpair = words.slice(1, words.size).map(friend => {
        if (key < friend) (key, friend) else (friend, key)
    })
    fpair.map(pair => (pair, words.slice(1, words.size).toSet))
}

def Reducer(accumulator: Set[String], set: Set[String]) = {
    accumulator intersect set
}

val data = sc.textFile("out.facebook-wosn-links")
val results = data.flatMap(Mapper)
    .reduceByKey(Reducer)
    .filter(!_._2.isEmpty)
    .sortByKey()

results.saveAsTextFile("friend_e4b")

results.collect.foreach(line => {
    println(s"${line._1} ${line._2.mkString(" ")}")
})
