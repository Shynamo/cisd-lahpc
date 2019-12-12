currDir=$(pwd)

mkdir -p build && cd build && \
cmake .. && make -j8

if ! [ $? -eq 0 ]; then
    echo "Please run this script from the project root directory."
    rm -r build
    cd $currDir
    exit 1
fi

cd $currDir