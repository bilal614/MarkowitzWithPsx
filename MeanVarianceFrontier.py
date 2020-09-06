import matplotlib.pyplot as plt
import csv
from os import path

x = []
y = []

filePath = path.join("output", "MeanVarianceFrontier.csv")
with open(filePath,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    next(plots)
    for row in plots:
        x.append(float(row[0]))
        y.append(float(row[1]))



plt.style.use('seaborn-whitegrid')
plt.scatter(x,y, color='blue', marker='o', label='Loaded from file:' + filePath)
plt.xlabel('Portfolio Variance')
plt.ylabel('Portfolio Return')
plt.title('Mean Variance Frontier')
plt.legend()


efficient_x = []
efficient_y = []
efficientFrontierFilePath = path.join("output", "EfficientFrontier.csv")
with open(efficientFrontierFilePath,'r') as csvfile:
    plots = csv.reader(csvfile, delimiter=',')
    next(plots)
    for row in plots:
        efficient_x.append(float(row[0]))
        efficient_y.append(float(row[1]))

plt.plot(efficient_x, efficient_y, color='red')

plt.show()
