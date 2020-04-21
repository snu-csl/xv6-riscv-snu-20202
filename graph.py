#!/usr/bin/python3

#----------------------------------------------------------------
#
#  4190.307 Operating Systems (Spring 2020)
#
#  Project #4: Simplified Linux 2.4 Scheduler
#
#  April 19, 2020
#
#  By Seong-Yeop Jeong
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#
#----------------------------------------------------------------

import matplotlib
matplotlib.use('Agg');
import matplotlib.pyplot as plt
import pandas as pd
import re
import sys

def pre_pros(data):
    lines = open(data, "r+").readlines()
    data = []
    
    for line in lines:
        line_ = re.findall("\d+", line)
        if(len(line_) == 4):
            data.append(line.rstrip('\n').split(','))

    column = ['time', 'P1', 'P2', 'P3']
    df = pd.DataFrame(data, columns=column)
    df = df.astype(str).astype(int)
    df = df.set_index("time")

    return df

def plot_df(df, x, y):
    plt.plot(df.index, df.P1, marker='o', label = 'P1')
    plt.plot(df.index, df.P2, marker='o', label = 'P2')
    plt.plot(df.index, df.P3, marker='o', label = 'P3')
    plt.xticks([i for i in range(0,x,20)])
    plt.yticks([i for i in range(0,y,5)])
    plt.grid()
    plt.legend(loc='upper center', ncol=3)
    plt.title('Ticks')
    #plt.ylim(ymin=0)
    #plt.xlim(xmin=0)
    plt.savefig(fig, format='png')
    #plt.show()
    plt.close()

    return

if __name__=="__main__":
    if(len(sys.argv) > 3):
        print("usage : ./graph.py xv6.log graph.png")
    data = sys.argv[1] if len(sys.argv) > 1 else 'xv6.log'
    fig = sys.argv[2] if len(sys.argv) > 2 else 'graph.png'
    df = pre_pros(data)
    plot_df(df,180,45)
    print("graph saved in the '%s' file" % fig);
