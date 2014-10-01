rm -f main-ok main-bad sha.so shb.so

echo building...
gcc -shared -fPIC -o sha.so sha.c
gcc -shared -fPIC -o shb.so shb.c
gcc -fPIC -o main-ok  main-ok.c -ldl
gcc -fPIC -o main-bad main-bad.c -ldl
