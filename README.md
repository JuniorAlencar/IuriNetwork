## Network model ALPHA-BETA
## Requirements
- cmake

- g++ compile

- boost library

- zlib library


# To install *cmake* (linux ubuntu)
```bash
sudo apt install cmake  
```

# To install *boost* (linux ubuntu)
```bash
sudo apt install libboost-all-dev  
```

# To install *zlib* (linux ubuntu)
```bash
sudo apt install zlib1g-dev
```

## Running code
```bash

# Navigate to the folder where the clone was made
cd TsallisNetwork

# Create build folder to generate executable
mkdir build && cd build

# Generate the cmake files
cmake ..

# Generate the executable
make -j

# The exe1 executable will be generated, the input parameters must be the standard expressed in example.json. To execute the code, we must follow the pattern
path_to_executable/exe1 N alpha seed N_0
```