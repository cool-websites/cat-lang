# cat

A small, standalone, fast, and lightweight programming language with the `.cat` extension.

## Build

```bash
mkdir build
cd build
cmake ..
make
```

## Run

```bash
./cat ../test/hello.cat
```

## Example

```cat
x = 42;
y = x + 8;
print(x);
print(y);
```