def lineParse(line):
    indexColon = line.index(":")
    lineSplit = line[(indexColon+1):].split()
    lineNoDirty = filter(lambda i: i != 'D', lineSplit)
    numBlocks = len(lineNoDirty)
    dirtyMap = [0] * numBlocks;
    for i in range(0, numBlocks):
        pos = lineSplit.index(lineNoDirty[i])
        if (pos+1 < len(lineSplit)) and (lineSplit[pos+1] == 'D'):
            dirtyMap[i] = 1;
    return  [lineNoDirty, dirtyMap]    

def compareLine(lineStd, lineInput):
    numBlocks = 0
    numTagWrong = 0
    numDirtyWrong = 0
    [lineNoDirtyStd, dirtyMapStd] = lineParse(lineStd)
    [lineNoDirtyInput, dirtyMapInput] = lineParse(lineInput)
    numBlocks = len(lineNoDirtyStd)
    if (len(lineNoDirtyStd) != len(lineNoDirtyInput)):
        numTagWrong = numBlocks
        return [numBlocks, numTagWrong, numDirtyWrong]
    for i in range(0, numBlocks):
#	print lineNoDirtyInput[i].lower()#
#	print lineNoDirtyStd[i] #
        if (lineNoDirtyStd[i] != lineNoDirtyInput[i].lower()):
            numTagWrong = numTagWrong + 1
        if (dirtyMapStd[i] != dirtyMapInput[i]):
            numDirtyWrong = numDirtyWrong + 1
    return [numBlocks, numTagWrong, numDirtyWrong]

def orderMismatchCheck(lineStd, lineInput):
    [lineNoDirtyStd, dirtyMapStd] = lineParse(lineStd)
    [lineNoDirtyInput, dirtyMapInput] = lineParse(lineInput)
    numBlocks = len(lineNoDirtyStd)
    numWrong = 0
    lineNoDirtyInput = list(set(lineNoDirtyInput))
    numWrong = numWrong + numBlocks - len(lineNoDirtyInput)
    for each in lineNoDirtyInput:
        try:
            lineNoDirtyStd.index(each.lower())
        except ValueError:
            numWrong = numWrong + 1
    return numWrong

def getAndCompareStats(lineStd, lineInput):
    indexColon = lineStd.index("=")
    lineSplitStd = lineStd[(indexColon+1):].split()
    indexColon = lineInput.index("=")
    lineSplitInput = lineInput[(indexColon+1):].split()
    return [lineSplitStd[0].rstrip('%'), lineSplitInput[0].rstrip('%')]


def gradeOutputFiles(fileName1, fileName2):
    f1 = open(fileName1, 'r')   # std file....
    f2 = open(fileName2, 'r')   # your inputs....
    Grades = 100
    CHECK_STAT_FLAG = False
    CHECK_CONTENT_FLAG = True
    totStats = 0
    totWrongStats = 0
    totContents = 0
    totWrongContents = 0
    while True:
        line1 = f1.readline()
        line2 = f2.readline()
	#print line1, line2
        if not line1: 
            break
        if not line2:
            break
        while (line1 == '\n' or len(line1.split()) == 0):
            line1 = f1.readline()
        while (line2 == '\n' or len(line2.split()) == 0):
            line2 = f2.readline()
        try:
            line1.index("CONFIGURATION")
        except ValueError:
            pass    # Do Nothing
        else:
            CHECK_STAT_FLAG = False
	    CHECK_CONTENT_FLAG = False
            continue
        try:
            line1.index("RESULTS")
        except ValueError:
            pass    # Do Nothing
        else:
            CHECK_CONTENT_FLAG = False
            CHECK_STAT_FLAG = True
            continue
        if CHECK_STAT_FLAG is True:
            totStats = totStats + 1
            [stat1, stat2] = getAndCompareStats(line1, line2)
            if (abs(float(stat1) - float(stat2)) > 1e-2):
                totWrongStats = totWrongStats + 1
            continue
        if CHECK_CONTENT_FLAG is True:
		for i in range(0,9):
            		x1 = line1.split()[i]
            		x2 = line2.split()[i]
            		totContents = totContents + 1
            		if (x1 == x2) is False:
                		totWrongContents = totWrongContents + 1
    f1.close()
    f2.close()
    #print totStats, totWrongStats, float(totWrongStats)/float(totStats)
    return [totStats, totWrongStats, totContents, totWrongContents]

def gradeCacheOutputFiles(fileName1, fileName2):
    f1 = open(fileName1, 'r')   # std file....
    f2 = open(fileName2, 'r')   # your inputs....
    Grades = 100
    CHECK_STAT_FLAG = False
    CHECK_CACHE_FLAG  = False
    CHECK_CONTENT_FLAG = True
    totStats = 0
    totWrongStats = 0
    totContents = 0
    totWrongContents = 0
    totCacheContents = 0
    totWrongCacheContents = 0
    while True:
        line1 = f1.readline()
        line2 = f2.readline()
	#print line1, line2
        if not line1: 
            break
        if not line2:
            break
        while (line1 == '\n' or len(line1.split()) == 0):
            line1 = f1.readline()
        while (line2 == '\n' or len(line2.split()) == 0):
            line2 = f2.readline()
        try:
            line1.index("CONFIGURATION")
        except ValueError:
            pass    # Do Nothing
        else:
            CHECK_STAT_FLAG = False
	    CHECK_CONTENT_FLAG = False
	    CHECK_CACHE_FLAG = False
            continue
        try:
            line1.index("RESULTS")
        except ValueError:
            pass    # Do Nothing
        else:
            CHECK_CONTENT_FLAG = False
            CHECK_STAT_FLAG = True
	    CHECK_CACHE_FLAG = False
            continue
        try:
            line1.index("CONTENTS")
        except ValueError:
            pass    # Do Nothing
        else:
            CHECK_CONTENT_FLAG = False
            CHECK_STAT_FLAG = False
	    CHECK_CACHE_FLAG = True
            continue

        if CHECK_STAT_FLAG is True:
            totStats = totStats + 1
            [stat1, stat2] = getAndCompareStats(line1, line2)
            if (abs(float(stat1) - float(stat2)) > 1e-2):
                totWrongStats = totWrongStats + 1
            continue
        if CHECK_CONTENT_FLAG is True:
		for i in range(0,9):
            		x1 = line1.split()[i]
            		x2 = line2.split()[i]
            		totContents = totContents + 1
            		if (x1 == x2) is False:
                		totWrongContents = totWrongContents + 1
        if CHECK_CACHE_FLAG is True:
            [numCacheContents, numWrongCacheContents, unusedParam] = compareLine(line1, line2)
            totCacheContents = totCacheContents + numCacheContents
            totWrongCacheContents = totWrongCacheContents + numWrongCacheContents
    f1.close()
    f2.close()
    #print totStats, totWrongStats, float(totWrongStats)/float(totStats)
    return [totStats, totWrongStats, totContents, totWrongContents, totCacheContents, totWrongCacheContents]


def gradingPolicy(totStats, totWrongStats, totContents, totWrongContents):
    pctCorrectStats = 1 - float(totWrongStats) / float(totStats)
    pctCorrectContents = 1 - float(totWrongContents) / float(totContents)
    Grades = 91.0*pctCorrectContents + 9.0*pctCorrectStats
    return Grades

def gradingCachePolicy(totStats, totWrongStats, totContents, totWrongContents, totCacheContents, totWrongCacheContents):
    pctCorrectStats = 1 - float(totWrongStats) / float(totStats)
    pctCorrectContents = 1 - float(totWrongContents) / float(totContents)
    pctCorrectCacheContents = 1 - float(totWrongCacheContents) / float(totCacheContents)
    Grades = 43*pctCorrectCacheContents + 43.0*pctCorrectContents + 14.0*pctCorrectStats
    return Grades

def printGrades(fPtr, ValidationFile, Grades, totContents, totWrongContents, totStats, totWrongStats, totCacheContents, totWrongCacheContents):
    strTmp = "Validation_Run:  " + ValidationFile + ":\n"
    strTmp = strTmp + "Grades: "
    fPtr.write(strTmp)
    fPtr.write(str(Grades))
    strTmp = "\nTotal Contents = " + str(totContents) + "\n"
    fPtr.write(strTmp)
    strTmp = "Total Wrong Contents = " + str(totWrongContents) + "\n"
    strTmp = strTmp + "Total Stats = " + str(totStats) + "\n"
    strTmp = strTmp + "Total Wrong Stats = " + str(totWrongStats) + "\n"
    strTmp = strTmp + "Total CacheContents = " + str(totCacheContents) + "\n"
    strTmp = strTmp + "Total Wrong CacheContents = " + str(totWrongCacheContents) + "\n"
    fPtr.write(strTmp)
    fPtr.write("\n\n")


