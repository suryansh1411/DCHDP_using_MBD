from cProfile import label
import sys, os
import csv
import pandas as pd
import numpy as np
# from scipy.special import comb
from math import *
from array import array

def comb(arr, n):
    ans = []
    for i in arr:
        ans.append((i*(i-1))/2)
    return np.array(ans)

def rand_index(actual, pred):    #first argument: actual clusterid list of all points and second argument: predicted clusterid list
    tp_plus_fp = comb(np.bincount(actual), 2).sum()
    tp_plus_fn = comb(np.bincount(pred), 2).sum()
    A = np.c_[(actual, pred)]
    tp = sum(comb(np.bincount(A[A[:, 0] == i, 1]), 2).sum()
             for i in set(actual))
    fp = tp_plus_fp - tp
    fn = tp_plus_fn - tp
    n = len(A)
    tn = (n*(n-1))/2- tp - fp - fn
    return (tp + tn) / (tp + fp + fn + tn)
    # return 0



# runs = 1
dataFile = str(sys.argv[1])
percent = float(sys.argv[2])
minpts = int(sys.argv[3])
# percent = 36.7
# minpts = 1
numClasses = int(sys.argv[4])


#compile
# os.system('g++ ./main.cpp ./data.cpp  ./dchdp.cpp ./treenode.cpp ./itree.cpp ./iforest.cpp')

#dchdp
os.system('./a.out ' + dataFile + ' ' + str(percent) + ' ' + str(minpts) + ' ' + str(numClasses))
#dbscan
# os.system('./a.out ' + dataFile + ' ' + str(percent) + ' ' + str(minpts))

#dchdp
labelsFile = r'clusterings/dchdp_'+dataFile[10:-4]+'_'+str(percent)+'_'+str(minpts)+'_'+str(numClasses)+'.csv'
#dbscan
# labelsFile = r'clusterings/dbscan_'+dataFile[10:-4]+'_'+str(percent)+'_'+str(minpts)+'.csv'

clustering_output = pd.read_csv(labelsFile, sep='\t')
clusters = clustering_output.groupby('actual')['predicted'].apply(list)
classes = clustering_output.groupby('predicted')['actual'].apply(list)

#f measure
precision= []
recall = []
for class_i in classes:
    class_i = pd.DataFrame(class_i)
    class_i = class_i[0].value_counts()
    precision_i = class_i.max() / class_i.sum()
    recall_i = class_i.max() / len(clusters[class_i.idxmax()])
    precision += [precision_i]
    recall += [recall_i]
precision = np.array(precision)
recall = np.array(recall)
f_measure  = np.average((2 * precision*recall)/(precision+recall))
print('f-measure :', f_measure)


#entropy
cluster_entropy=[]
for cluster in clusters:
    cluster = pd.DataFrame(cluster)
    classes=cluster[0].value_counts()
    classes = classes/float(cluster.count())
    e = (classes * [log(x, 2) for x in classes]).sum()
    cluster_entropy += [-e]
cluster_size = np.array([len(c) for c in clusters])
cluster_fraction = cluster_size/float(cluster_size.sum())
entropy = (cluster_fraction * cluster_entropy).sum()
print ('entropy :', entropy)

#rand index
r_index = rand_index(clustering_output['actual'], clustering_output['predicted'])  
print("rand index :", r_index)