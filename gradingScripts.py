import os
import sys
import traceback
#sys.path.append(".")
from compareOutputs import *
#os.system("tcsh")

grades = 0.0
Section = 563  ## or 563

if (os.path.isdir("./YourTestRuns") == True):
    os.system("rm -rf ./YourTestRuns")

if (os.path.isdir("./YourTestRuns") == False):
    os.mkdir("YourTestRuns")

f = open ('transcripts.txt', 'w')
f.write("Transcripts for Project 3\n\n")

try:
    os.system("make clean; make")
except TypeError:
    f.write("Unable to compile....\nGrades: 0.00\n")
    f.close()
    exit()

if not os.path.exists("./sim"):
    f.write("Unable to compile....\nGrades: 0.00\n")
    f.write("Total Grades: ")
    f.write("0.0")
    f.write("\n")
    f.close()
    f = open('UnableToCompile.txt', 'w')
    f.write("Mark....")
    f.close()
    exit()

## ValidationRun
os.system("./sim 16 4 0 0 0 0 0 ./traces/val_gcc_trace_mem.txt > ./YourTestRuns/YourTestRun1.txt")
os.system("./sim 32 16 0 0 0 0 0 ./traces/val_perl_trace_mem.txt > ./YourTestRuns/YourTestRun2.txt")
os.system("./sim 16 4 32 2048 8 0 0 ./traces/val_gcc_trace_mem.txt > ./YourTestRuns/YourTestRun3.txt")
os.system("./sim 32 8 32 1024 4 2048 8 ./traces/val_perl_trace_mem.txt > ./YourTestRuns/YourTestRun4.txt")


## Grading Phase
## We have mystery runs, so your actual grades might vary if it fails in Mystery Run
## Also, for some students, your cache blocks may be correct but with wrong orders. I'm yet to update the grading rubrics for those partial grades in this script. However, partial grades will be given for minor mistakes....
#os.system("diff -iw ./YourTestRuns/YourTestRun1.txt ./Validation_Runs/val_1.txt > ./YourTestRuns/diff1.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun2.txt ./Validation_Runs/val_2.txt > ./YourTestRuns/diff2.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun3.txt ./Validation_Runs/val_extra_1.txt > ./YourTestRuns/diff3.txt")
#os.system("diff -iw ./YourTestRuns/YourTestRun4.txt ./Validation_Runs/val_extra_2.txt > ./YourTestRuns/diff4.txt")	

totStats = []
totWrongStats = []
totContents = []
totWrongContents = []
totCacheContents = []
totWrongCacheContents = []
Grades = []

#if(os.path.getsize('./YourTestRuns/diff1.txt')==0)


ValidationFiles = ["val_1.txt", "val_2.txt", "val_extra_1.txt", "val_extra_2.txt"]
for i in range(0,4):
    successFlag = True
    try:
        fileObj1 = "YourTestRuns/YourTestRun" + str(i+1) + ".txt"
        fileObj2 = "Validation_Runs/" + ValidationFiles[i]
        if (i < 2):
            [totStats1, totWrongStats1, totContents1, totWrongContents1] = gradeOutputFiles(fileObj1, fileObj2)
        else:
            [totStats1, totWrongStats1, totContents1, totWrongContents1, totCacheContents1, totWrongCacheContents1] = gradeCacheOutputFiles(fileObj1, fileObj2)
    except:
	traceback.print_exc()
        successFlag = False
        totStats.append("NA")
        totWrongStats.append("NA")
        totContents.append("NA")
        totWrongContents.append("NA")
        totCacheContents.append("NA")
        totWrongCacheContents.append("NA")
        Grades.append(0)
        continue
    else:
        totStats.append(totStats1)
        totWrongStats.append(totWrongStats1)
        totContents.append(totContents1)
        totWrongContents.append(totWrongContents1)
        if (i < 2):
            totCacheContents.append("NA")
            totWrongCacheContents.append("NA")
        else:
            totCacheContents.append(totCacheContents1)
            totWrongCacheContents.append(totWrongCacheContents1)
    try:
        if (i<2):
            grades1 = gradingPolicy(totStats1, totWrongStats1, totContents1, totWrongContents1)
        else:
            grades1 = gradingCachePolicy(totStats1, totWrongStats1, totContents1, totWrongContents1, totCacheContents1, totWrongCacheContents1)

    except:
        successFlag = False
        Grades.append(0)
    else:
        Grades.append(grades1)

print "Show Grades:"
for i in range(0,4):
    printGrades(f, ValidationFiles[i], Grades[i], totContents[i], totWrongContents[i], totStats[i], totWrongStats[i], totCacheContents[i], totWrongCacheContents[i])
    print ValidationFiles[i], ": ", Grades[i]

totGrades = 50.0
for i in range(0,2):
        totGrades = totGrades + 15 * float(Grades[i]) / 100.0;

bonusGrades = 0.0
for i in range(2,4):
        bonusGrades = bonusGrades + 5.0 * float(Grades[i]) / 100.0;

f.write("Total Coding Part Full Credit is 80+10 for both ECE463 and ECE563 Students")
f.write("\n")
f.write("Normal Grades: ")
#f.write(str(grades))
f.write(str(totGrades))
f.write("\n")
f.write("Bonus Grades: ")
f.write(str(bonusGrades))
f.write("\n")
f.write("Total Grades: ")
f.write(str(totGrades+bonusGrades))
f.close()
print "(Total Coding Part Full Credit is 80+10 for both ECE463 and ECE563 Students)"
print totGrades+bonusGrades, " (", totGrades, " + ", bonusGrades, ")"


