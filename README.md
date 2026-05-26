# HashMap-HashingFunction-Demo

This repository is a small, intentionally-over-simple C++/CMake project for teaching:

- A `Dictionary` class implemented with a **hash-driven nibble trie** (4 bits at a time)
  - Keys are hashed to a 64-bit integer
  - Each 4-bit nibble chooses a branch
  - Each level's branches are stored as a small **linked list of siblings**
- A CMake project layout (`src/`, `include/`, `examples/`, `tests/`)
- Unit testing with **GTest**
- Can be consumed as a library by other CMake projects (via `FetchContent` or `find_package`)

## Build

```sh
cmake -S . -B build
cmake --build build
```

## Run the example

```sh
./build/examples/dictionary_example
```

## Run the profiling example

```sh
./build/examples/profile_dictionary
```

You can increase the workload to make the slowness more obvious:

```sh
# args: [ops] [updateProbability] [maxInitialKeys]
./build/examples/profile_dictionary 200000 0.7 20000
```

## Run tests

```sh
ctest --test-dir build
```

## Consuming from another CMake project

### Option A: FetchContent (no install)

```cmake
include(FetchContent)

FetchContent_Declare(
  Dictionary
  GIT_REPOSITORY https://github.com/AbstractClassroom-CPP/HashMap-HashingFunction-Demo.git
  GIT_TAG v0.2.0
)
FetchContent_MakeAvailable(Dictionary)
target_link_libraries(your_app PRIVATE Dictionary::Dictionary)
```

This project uses `SimpleIntegerHash` via `FetchContent`, so using `FetchContent` is the
easiest way to consume it.

### Option B: Install + find_package

```sh
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=./install
cmake --build build
cmake --install build
```

Then in your consuming project:

```cmake
find_package(Dictionary CONFIG REQUIRED)
target_link_libraries(your_app PRIVATE Dictionary::Dictionary)
```

Note: the installed package expects `SimpleIntegerHash` to also be findable via
`find_package(SimpleIntegerHash CONFIG REQUIRED)`.

