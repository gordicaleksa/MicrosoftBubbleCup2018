# Solutions for Microsoft Bubble Cup 2018
This is my first time competing on Microsoft Bubble Cup.

Unfortunately 2 of my team members didn't have the time to compete so I was left all by myself.
So I am really using this competition to learn new algortihms,  theory and mathematics as it is quite hard to compete against multiple (up to 3) member teams.

My personal rank in round 1 was 38. out of 425 competitors, 
as I didn't have enough time to solve 4 other problems and optimize the challenge problem.

Link to [Microsoft Bubble Cup](http://bubblecup.petlja.org/) web site.

## What I "learned" from the round 1 problems

### Big Snowfall ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p1_BigSnowfall.cpp))
Rudimentary graph problem I brushed up my knowledge about:
* Weakly and strongly connected components in directed graph and connected components in undirected graph.
* Conditions for Eulerian cycle and Eulerian path in directed and undirected graph
### Find The Next Letter ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p5_FindTheNextLetter.cpp))
This was a very simple problem, brute-force method sufficed.
### Oil Skimming ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p6_OilSkimming.cpp))
Really interesting problem as it has a brilliant solution. Max flow problem in disguise. 
I first tried greedy approach and quickly found out it doesn't give correct results when patterns like this occur:

    ### and .##
    ##.     ###
    .#.     ..#
Later I tried to recurse but that solution produced TLE (time limit exceeded) as I already expected.

Then I found out that this is actually a simple fitting problem and that if you imagine a chess board pattern behind all these '.'  and '#' symbols you can actually build a bipartite graph G=(U,V,E) out of it. White tiles belong to U set while black tiles belong to set V. Then all you have to do is find the maximum matching for this bipartite graph and you will optimize the number of oil slicks you cover. Simple max-flow scheme like Ford-Fulkerson does the job ( I used Edmonds-Karp algorithm)
### Optimum Click ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p7_OptimumClick.cpp))
This was also pretty much a simple problem, but I actually ended up having some problems with it because of the grading system which always gives either TLE or WA (wrong answer) even though in many cases the problem is RTE (run-time error). It's important to notice that k is to big to allocate array of that size and you don't need that for algorithm to be correct in the first place, but hey we are humans we make silly mistakes sometimes.

### Origami ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p8_Origami.cpp))
Proved to be the most difficult problems in round 1. Computational geometry problem.
Things I learned:

 - Efficient convex hull algorithms (n log n)
 - MBR (minimum bounding rectangle) algorithm
 - rotating calipers method (n log n)

I ended up having to read a [paper](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.155.5671&rep=rep1&type=pdf) by Godfried Toussaint about rotating calipers and implement the algorithm.

Debugging was hell, probably the most intensive debugging of my life. I ended up writing brute force O(n<sup>2</sup>) algorithm and used it as reference algorithm. I also automated testing using files and random generators. I would give the number of points and coordinates ranges to a function that initialized a file and then brute force and rotating calipers would read from that file.

I noticed that for coordinates bigger than 10<sup>5</sup> results of these 2 algorithms start to differ (for smaller values it worked like a charm).
It occurred to me that I could scale the points I got from the convex hull algo, then apply the rotating calipers and then just simply multiple the area by the square of the scale factor. Finally the results were always correct.

### Real Phobia ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p9_RealPhobia.cpp))
What I learned:

 - continued fractions, convergents, semi-convergents
 - Stern-Brocot tree
 - Farey sequence

I first tried to solve this problem by doing a binary fashion Stern-Brocot tree traversal. This [paper](https://pdfs.semanticscholar.org/8500/00d79acaf10cb8c601ab675ce3f06cf40da6.pdf) helped a lot. I got TLE. 

Then I tried to implement a binary fashion Stern-Brocot compressed tree traversal, again I failed miserably, TLE.

Then I realized I had to brush up my continued fractions knowledge so I read through [this web site](http://www.maths.surrey.ac.uk/hosted-sites/R.Knott/Fibonacci/cfINTRO.html), it really helped a lot. I realized a connection between the Euclidean algorithm and continued fractions. [This page](https://shreevatsa.wordpress.com/2011/01/10/not-all-best-rational-approximations-are-the-convergents-of-the-continued-fraction/) and [this wiki page](https://en.wikipedia.org/wiki/Continued_fraction#Best_rational_approximations) are also good resources. Armed with this knowledge I implemented the algorithm. Result? WA. I was stunned. After a couple of hours of debugging I gave up. Tomorrow I returned and I just tried to delete the special case I read on wiki and it worked. The grading system accepts the solution when you give it the closest semi-convergent even though that's not always the best solution... 
### Rocks ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p10_Rocks.cpp))
Relatively easy problem. A couple of small optimizations were needed like storing the weights of rock names in a vector so that you can quickly execute "M X Y" command. Here I missed the fact that X doesn't necessarily need to be left of Y in the array so I ended up giving 0 on those cases which caused some painful debugging.
### Fire Evacuation Plan ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p11_FireEvacuationPlan.cpp))
I will also post this solution because I think it's quite neat even though it was suboptimal and I got TLE on test case  4 (the algorithm is correct though). I implemented Z-algorithm and then used it's output to find the number of different destinations.
### Challenge problem ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round1/p12_Challenge.cpp))
Challenge problem was a glorified general (meaning triangle inequality doesn't hold) 
STSP (symmetric travelling salesman problem). I didn't have a lot of time to optimize it so I ended up having 4791 points while the best teams were in the range 6000. They actually implemented the same algorithm at the end but had more time to play with parameters. Data set contained up to 400 points so only heuristic approaches worked. (exact algorithms like brute force O(n!), Held-Karp O(n<sup>2</sup>*2<sup>n</sup>), cutting plane method wouldn't pass the grading system)

Heuristics I tried (from more simple ones to more complicated ones):

 - Choose random permutation of points, do this n<sup>2</sup> (n = number of points) times and output the path with smallest weight (this gave about 700 points)
 - build MST (minimum spanning tree) using either Prim's or Kruskal's algorithm and then do the preorder traversal. This actually only works as 2-approximation algo if the triangle inequality holds (which it doesn't). The plan was to traverse MST in different way than using simple preorder traversal. (this gave around 500 points, I guess that Christofides 3/2-approximation algo wouldn't do any better)
 - NN heuristic (nearest neighbor greedy algorithm). Choose random point as starting point. Find the closest not visited point to it and proceed to that point. Repeat. (this gave around 2500 points)
 - Greedy heuristic. NN resembles Prim's algorithm while this algo resembles Kruskal's algorithm. We choose the smallest edge that doesn't create a cycle and doesn't cause a node to have a degree greater than 2. Repeat. (gave wrong answer because it gave very long paths, bad heuristic for this data set)
 - 2-opt heuristic
 - Lin & Kernighan algorithm (even today state of the art almost all of the top teams implemented it)

There are a lot of other heuristics and approaches out there. Genetics algorithms were to slow for this grading system (I am not sure about Ant colony optimization), simulated annealing also gave bad results. I wonder if large step Markov chain algorithm could have done better than L&K.
[This resource](https://web.tuke.sk/fei-cit/butka/hop/htsp.pdf) helped me as well as this [GitHub repo]( https://github.com/lingz/LK-Heuristic/blob/master/LKMatrix.cpp) for implementing L&K in short time. There are a lot of good resources out there as TSP is a problem that many other problems can be reduced to, so different heuristics are often invented precisely for TSP.

## What I "learned" from the round 2 problems

### Ada and Cucumber ([code](https://github.com/gordicaleksa/MicrosoftBubbleCup2018/blob/master/Round2/p9_AdaAndCucumber.cpp))
Computational geometry problem. Basically the problem was you are given a bunch of line segments on a 2D Cartesian plane and you are interested in how many of them (maximum) can you intersect if you choose a direction starting from origin (0,0) wisely.

My first (naive) approach was somewhat computationally (time) intensive hence I got TLE. I would create a half-line starting from origin and going outwards and then count how many of the segments it intersects. Then I would incrementally increase the angle so that I swipe all of the 360 degrees. So the time-complexity was O(k*n) where k - number of increments and n - number of line segments (up to 10<sup>6</sup>).

My second approach is somewhat similar to the line-sweep algorithm. I would first map the segments to angles they make with the origin thus basically reducing the problem to 1D. Than I would sort those angles and I used them as events when counting the maximum number of overlaps (which corresponds to the maximum number of lines I would intersect going from the origin outwards in certain direction).
