import json
import numpy as np

from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report
from sklearn.model_selection import train_test_split

import re
import pandas as pd

def eval_svm():
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open('../analysed.out', 'r') as file:
            content = file.read()
            traces = re.findall(r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)', content)
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6])
                })
        
        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance']]
        y = df['Label']
        
        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )
        
        # Step 3: Train classifier using SVM
        classifier = SVC(kernel='linear', random_state=i)  # You can try different kernels (e.g., 'rbf', 'poly')
        classifier.fit(X_train, y_train)
        
        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)
        
        # Do not modify the next two lines
        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))

def eval_RFC():
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open('../analysed.out', 'r') as file:
            content = file.read()
            traces = re.findall(r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)', content)
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6])
                })
        
        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance']]
        y = df['Label']
        
        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )
        
        # Step 3: Train classifier using Random Forest
        classifier = RandomForestClassifier(n_estimators=100, random_state=i)
        classifier.fit(X_train, y_train)
        
        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)
        
        # Do not modify the next two lines
        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))
    
def eval_KNN():
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open('../analysed.out', 'r') as file:
            content = file.read()
            traces = re.findall(r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)', content)
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6])
                })
        
        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance']]
        y = df['Label']
        
        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )
        
        # Step 3: Train classifier using KNN
        classifier = KNeighborsClassifier(n_neighbors=11)  # Use 5 neighbors by default
        classifier.fit(X_train, y_train)
        
        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)
        
        # Do not modify the next two lines
        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))
    

if __name__ == "__main__":
	eval_RFC()
