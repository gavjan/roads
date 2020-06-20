# Roads
Implementation for constructing a national road map supporting operations such as:
- Generating a shortest path between two cities
- Exteneding an existing path to accommodate another city while having the shorest paths between the cities
- Removing a portion of a road while making sure that every path invovling that road is efficiently redirected

Shortest pathfinding between two cities is achieved with the help of Dijkstras's algorithm with a slight modification to suit Double-Weighted Graphs. Find the detailed description (in Polish) in the file `task.pdf`

## Documentation
The project supports automatic documentation generation with `Doxygen`.

## Build
From the directory of the project run:
```
cd src && cmake .. && cd ..
```
To generate documentation in Polish run:
```
cd src && doxygen
```
To generate documentation in English run:
```
echo "OUTPUT_LANGUAGE        = English" >> Doxyfile.in
cd src && doxygen
```


