# Stanford University
## CS106L: Standard C++, Autumn 2019

## Assignment
In my computer system Windows 10, the Qt creater can not identify
**.pro* file's * tag, for instance, **.h*, **.cpp*. I just 
modify it to be concrete file name/path.

### Assignment 1: GraphViz
**Compute the repulsive force between every pair of nodes.**     
Using the following code to iterate every pair of nodes only once.    
```C++
// n is the number of nodes
for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
        // something
    }
}
```
