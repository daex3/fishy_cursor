# Fishy cursor
Simple game: Tons of Goldfish will swim around you(their food) Requires CJSON+Tegrine
## Setup
```shell
git clone https://github.com/daex3/tegrine ../tegrine
sudo ln -s $(PWD)/../tegrine /usr/include/tegrine
mkdir -p ~/.tegrine/saved
cp goldfish ~/.tegrine/saved
make
```