val fileFriends = sc.textFile("out.facebook-wosn-links")
val result = fileFriends.flatMap(Map)
    .reduceByKey(intersection)
    .filter(!_._2.equals("null")).filter(!_._2.isEmpty)
    .sortByKey()
val res = result.map(value => value._1 + "\t" + value._2.size)
res.saveAsTextFile("friend_e4c")

def intersection(first: Set[String], second: Set[String]) = {
    first.toSet intersect second.toSet
}

def Map(line: String) = {
    val line1 = line.split(" ")
    val person = line1(0)
    val newfriends = if (line1.length > 1) line1(1) else "null"
    val nfriends = newfriends.split(",")
    val friends = for (i <- 0 to nfriends.length - 1) yield nfriends(i)
    val pairs = nfriends.map(friend => {
      if (person < friend) person + "," + friend else friend + "," + person
    })
    pairs.map(pair => (pair, friends.toSet))     
}

res.foreach(println)
