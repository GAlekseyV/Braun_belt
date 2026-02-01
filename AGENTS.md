# Agent Guidelines for Braun Belt C++ Repository

This is a C++17 project for Yandex's "Brown Belt" course containing algorithm/data structure implementations and tests.

## Build Commands

```bash
# Build all targets
cd build && cmake .. && make

# Build specific task target
cd build && cmake .. && make Part_1_1_hash_set
cd build && cmake .. && make Part_2_1_spendings_json

# Target naming: {PART}_TASK_NAME (e.g., Part_1_1_hash_set, Part_2_2_ini)
```

## Test Commands

```bash
# Run all tests for a task (executables built in build/)
./build/Part_1_1_hash_set
./build/Part_2_0_spendings_xml

# Run single test: Temporarily comment out unwanted RUN_TEST lines in main(),
# or rename test function so only one RUN_TEST remains active
```

## Lint/Format Commands

```bash
# Format files (uses .clang-format config)
clang-format -i file.cpp
clang-format -i file.h

# Check with clang-tidy (uses .clang-tidy config)
clang-tidy file.cpp -- -std=c++17 -I test_framework/
```

## Code Style Guidelines

### Formatting (clang-format configuration)
- **Indentation**: 2 spaces, no tabs
- **Pointer alignment**: Right (`int* ptr` not `int *ptr`)
- **Braces**: Opening brace on new line for classes, functions, structs; same line for control statements
- **Column limit**: None
- **Short functions**: Allowed on single line

### Imports
- `using namespace std;` used at top of all files
- Include project headers first (`#include "test_runner.h"`), then standard library
- No import sorting enforced

### Naming Conventions
- **Classes**: `PascalCase` (e.g., `HashSet`, `PriorityCollection`, `Node`, `Document`)
- **Functions/Members**: `snake_case` or `PascalCase` - both observed, prefer consistency within file
- **Private members**: Leading underscore (`_buckets`, `_hasher`)
- **Types**: `PascalCase` aliases (`using Id = size_t;`, `using BucketList = forward_list<Type>;`)
- **Constants**: `UPPER_SNAKE_CASE` (not strongly observed)

### Types
- Use `size_t` for indices/sizes, `int` for integers
- Prefer `explicit` constructors to prevent implicit conversions
- Use `std::move()` for moving, `std::make_pair()`, `emplace_back()` where appropriate
- Use `[[nodiscard]]` on return values that should not be ignored
- Use `constexpr` and `noexcept` where appropriate (seen in hash functors)

### Error Handling
- Custom test framework provides `ASSERT(condition)` and `ASSERT_EQUAL(a, b)` macros
- Assertions throw `runtime_error` with file:line info on failure
- Catch generic `exception&` and `...` in TestRunner::RunTest
- TestRunner exits with code 1 if any test fails

### File Organization
- Separate `.h` and `.cpp` files for classes with implementations
- Test code in main `.cpp` file (test functions and main() together)
- Public methods first, then private section at bottom
- Type aliases (`using`) at top of class before public methods

### Testing Patterns
```cpp
void TestSmoke() { /* test implementation */ }

int main() {
  TestRunner trunner;
  RUN_TEST(trunner, TestSmoke);
  RUN_TEST(trunner, TestEmpty);
  return 0;
}
```

### Language
- Comments in Russian (course is in Russian)
- Use Russian comments when explaining logic, but code/identifiers in English