echo running ok...
LD_LIBRARY_PATH="." ./main-ok

echo running bad...
LD_LIBRARY_PATH="." ./main-bad

#echo clearing...
#rm main-ok main-bad sha.so shb.so
exit
