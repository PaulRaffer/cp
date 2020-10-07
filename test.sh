mkdir ./build
gcc -o ./build/cp ./src/cp.c
cd ./build
echo "Hello World!\n" > src.txt
./cp src.txt dest.txt
cat dest.txt

