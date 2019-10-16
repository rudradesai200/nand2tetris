#!/bin/bash
read foldername
read filename
cd ~/nand2tetris/nand2tetris/projects/08/$foldername
~/nand2tetris/nand2tetris/projects/08/VMTranslator.o $filename.asm $filename
~/nand2tetris/nand2tetris/tools/CPUEmulator.sh $filename.tst
#rm ~/nand2tetris/nand2tetris/projects/08/$foldername/$filename.asm
#rm ~/nand2tetris/nand2tetris/projects/08/$foldername/$filename.out
#a file to make work check easy
cd ~/nand2tetris/nand2tetris/projects/08/
