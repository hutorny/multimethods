## Chepurni multimethods for modern C++

This project implements multimethods with templates for C++17 and higher.
For a detailed description, please refer to [Chepurni multimethods for contemporary C++](doc/Chepurni multimethods for contemporary C++.pdf)

### `multimethod`

`multimethod` template dispatches calls to the first suitable function or a method from the list

Usage:
```cpp
  multimethod<ReturnType,
    &function1,
    &function2,
    &MyClass::method>::dispatch(a,b);
```

Requirements to the parameter classes:
* virtual table,  e.g. hierarchy defines at least one virtual method
* either enabled RTTI
* or hierarchy defines template instnace_of<Class>

### `matrixdispatch`

`matrixdispatch` template dispatches calls to the best suitable function or a method from the list

Usage:
```cpp
   matrixdispatch<FunctionType,
    &function1,
    &function2,
    &MyClass::method>::dispatch(a,b);
```

Requirements to the parameter classes:
* virtual method size_t classID()
* sequentially numbered classes
* backward mapping ID-to-class via DomainA, DomainB classes

`FunctionType` is a type of the functions, pointed from the matrix.

### Performance comparison

Performance is measured with `valgrind --tool=callgrind` running performance tests `perf1` and `perf2`.
Each test dispatches to 40 other functions. `(ir)` columns contain instructions per call,
`(s)` columns - the wall time per 10000000 loops (for these measurements it matches CPU time)
The test executable is build with option `NDEBUG=1`.
`std::visit` refers to a test, dispatching with `std::visit` over variant-of-objects,
while `std::visit*` - over variant-of-pointers.

|   Template      | gcc (ir) |clang (ir) | gcc (s)  | clang (s)|
|-----------------|---------:|----------:|---------:|---------:|
| `std::visit`    |    38    |      35   |    0.64s |    1.0s  |
| `std::visit*`   |    58    |      50   |    0.93s |    1.3s  |
| `multimethod`   |   750    |     702   |     5.8s |    6.4s  |
| `matrixdispatch`|    54    |      47   |    0.88s |    1.2s  |

### Notes
To compile with Visual Studio use following options `/std:c++17 /permissive-`
