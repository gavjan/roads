# Roads
The task implements operations on road maps. Find the detailed description in the file `task.pdf`

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


