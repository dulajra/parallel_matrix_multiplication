sample_size=100

echo "----- Running serial program with $sample_size samples -----"
echo
./bin/main_program.out s $sample_size

echo "----- Running parallel for program with $sample_size samples -----"
echo
./bin/main_program.out p $sample_size

echo "----- Running optimized parallel program with $sample_size samples -----"
echo
./bin/main_program.out m $sample_size