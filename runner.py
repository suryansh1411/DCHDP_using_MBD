import sys, os
import csv

dataFile = str(sys.argv[1])
numClasses = int(sys.argv[2])

#compile
os.system('g++ ./main.cpp ./data.cpp  ./dchdp.cpp ./treenode.cpp ./itree.cpp ./iforest.cpp')

# bestFscore = 0.0
# bestPercent = 0.0
# bestMinpts = 0.0

# for percentx in range(10, 30):
#     for minpts in range(2, 21):
#         percent = percentx / 10
#         filename = r'results/dbscan_' + dataFile[10:-4] + '_' + str(percent) + '_' + str(minpts) + '.csv'
#         # with open(filename, "r", encoding='ascii') as file:
#         #     read = csv.reader(file)
#         #     for row in read:
#         # print(filename)
#         os.system("rm -f "+ filename)
#         os.system('python3 -W ignore evaluation.py ' + dataFile + ' ' + str(percent) + ' ' + str(minpts) + ' >> ' + 'results/dbscan_' + dataFile[10:-4] + '_' + str(percent) + '_' + str(minpts) + '.csv')

#         with open(filename, "r", encoding='ascii') as file:
#             read = csv.reader(file)
#             row = next(read)
#             fscore = float(row[0].split(' ')[2])
#             print(fscore)
#             if fscore > bestFscore :
#                 bestFscore = fscore
#                 bestPercent = percent
#                 bestMinpts = minpts


# print("best fscore: ", bestFscore)
# print("best percent: ", bestPercent)
# print("best Minpts: ", bestMinpts)


bestFscore = 0.0
bestPercent = 0.0
bestEntropy = 0.0
bestRindex = 0.0
bestClusteringTime = 0.0
bestDisMatrixTime = 0.0

minpts = 1


for percentx in range(0, 1000):
    percent = percentx / 10.0
    filename = r'results/dchdp_' + dataFile[10:-4] + '_' + str(percent) + '_' + str(minpts) + '_' + str(numClasses) +'.csv'
    os.system("rm -f "+ 'results/dchdp_' + dataFile[10:-4] + '_' + str(percent) + '_' + str(minpts) + '_' + str(numClasses) +'.csv')
    os.system('python3 -W ignore evaluation.py ' + dataFile + ' ' + str(percent) + ' ' + str(minpts) + ' ' + str(numClasses) + ' >> ' + 'results/dchdp_' + dataFile[10:-4] + '_' + str(percent) + '_' + str(minpts) + '_' + str(numClasses) +'.csv')

    with open(filename, "r", encoding='ascii') as file:
        read = csv.reader(file)
        row = next(read)
        disMatrixTime = float(row[0].split(' ')[5])
        row = next(read)
        clusteringTime = float(row[0].split(' ')[3])
        row = next(read)
        fscore = float(row[0].split(' ')[2])
        row = next(read)
        entropy = float(row[0].split(' ')[2])
        row = next(read)
        r_index = float(row[0].split(' ')[3])
        # print(fscore)
        if fscore > bestFscore :
            bestFscore = fscore
            bestPercent = percent
            bestEntropy = entropy
            bestRindex = r_index
            bestClusteringTime = clusteringTime
        if bestDisMatrixTime == 0.0:
            bestDisMatrixTime = disMatrixTime

print()
print(dataFile)
print("best fscore: ", bestFscore)
print("best percent: ", bestPercent)   
print("best entropy: ", bestEntropy)
print("best rand index: ", bestRindex)
print("clustering time: ", bestClusteringTime)
print("distance matrix time: ", bestDisMatrixTime)    
print()
