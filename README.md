## Network model ALPHA-BETA
## Requirements
- cmake

- g++ compile

- boost library

- zlib library

- Apache arrow (to parquet)

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

# To install *apache arrow* (linux ubuntu)
```bash
sudo apt update
sudo apt install -y -V ca-certificates lsb-release wget

wget https://packages.apache.org/artifactory/arrow/$(lsb_release --id --short | tr 'A-Z' 'a-z')/apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb

sudo apt install -y -V ./apache-arrow-apt-source-latest-$(lsb_release --codename --short).deb

sudo apt update
sudo apt install -y -V libarrow-dev
```

## Running code
```bash

# Navigate to the folder where the clone was made
cd IuriNetwork

# Create build folder to generate executable
mkdir build && cd build

# Generate the cmake files
cmake ..

# Generate the executable
make -j

# The exe1 executable will be generated, the input parameters must be the standard expressed in example.json. To execute the code, we must follow the pattern
path_to_executable/exe1 N alpha seed N_0
```

