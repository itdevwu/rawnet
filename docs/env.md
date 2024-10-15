# Environment Setup

## Dependencies

### Python

Python >= 3.8

### Conan

```bash
pip install conan
```

### XMake

Pick one out of the following two methods to install xmake.

#### cURL

```bash
curl -fsSL https://xmake.io/shget.text | bash
```

#### wget

```bash
wget https://xmake.io/shget.text -O - | bash
```

## Build

### xmake (recommended)

```bash
xmake b
```

### CMake

```bash
xmake project -k cmake
mkdir build
cd build
cmake ..
make
```

### Clang (compiler_commands.json)

```bash
xmake project -k compile_commands
```

## Run

### XMake

Header files with static libraries of librawnet are by default under `build/` directory.

```bash
xmake r rawnet-test
```
