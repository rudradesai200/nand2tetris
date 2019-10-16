#!/bin/bash
read foldername
read filename
cd ~/nand2tetris/nand2tetris/projects/07/$foldername/
~/nand2tetris/nand2tetris/projects/07/VMTranslator.o $filename.asm $filename
~/nand2tetris/nand2tetris/tools/CPUEmulator.sh $filename.tst
rm ~/nand2tetris/nand2tetris/projects/07/$foldername/$filename.asm
rm ~/nand2tetris/nand2tetris/projects/07/$foldername/$filename.out
cd ~/nand2tetris/nand2tetris/projects/07/
#a file to make work check easy
