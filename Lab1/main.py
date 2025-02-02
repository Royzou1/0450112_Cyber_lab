import argparse
import re
import pandas as pd
import numpy as np

from sklearn.ensemble import RandomForestClassifier
from sklearn.svm import SVC
from sklearn.neighbors import KNeighborsClassifier
from sklearn.metrics import classification_report
from sklearn.model_selection import train_test_split


def eval_svm(file_path):
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open(file_path, 'r') as file:
            content = file.read()
            traces = re.findall(
                r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)',
                content,
            )
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6]),
                    'Max_Square': int(trace[2]) ** 2,
                    'Min_Square': int(trace[3]) ** 2,
                    'Min_Max': (int(trace[2]) - int(trace[3])) ** 2,
                    'Free_Ele': 1
                })

        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance', 'Max_Square', 'Min_Square', 'Min_Max', 'Free_Ele']]
        y = df['Label']

        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )

        # Step 3: Train classifier using SVM
        classifier = SVC(kernel='linear', random_state=i)
        classifier.fit(X_train, y_train)

        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)

        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))


def eval_rfc(file_path):
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open(file_path, 'r') as file:
            content = file.read()
            traces = re.findall(
                r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)\n\s+third_moment: ([\d.-]+)',
                content,
            )
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6]),
                    'Max_Square': int(trace[2]) ** 2,
                    'Min_Square': int(trace[3]) ** 2,
                    'Min_Mul_Max' : int(trace[2])*int(trace[3]),
                    'Min_Max': (int(trace[2]) - int(trace[3])) ** 2,
                    'third_moment': np.cbrt(float(trace[7])),
                    'CV' : ((float(trace[6]) ** 0.5) / float(trace[5])) * 100,
                    'Free_Ele': 1
                })

        # Print data to verify columns

        df = pd.DataFrame(data)

        try:
            X = df[['Min', 'Max','Min_Mul_Max','Min_Max',
                    'Median', 'Mean', 'Variance','third_moment', 'CV']]
            y = df['Label']
        except KeyError as e:
            print("KeyError:", e)
            continue

        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )

        # Step 3: Train classifier using Random Forest
        classifier = RandomForestClassifier(n_estimators=100, random_state=i)
        classifier.fit(X_train, y_train)

        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)

        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))

def eval_knn(file_path):
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open(file_path, 'r') as file:
            content = file.read()
            traces = re.findall(
                r'Trace (\d+) \(Label: (.*?)\):\n\s+Min: (\d+)\n\s+Max: (\d+)\n\s+Median: ([\d.]+)\n\s+Mean: ([\d.]+)\n\s+Variance: ([\d.]+)',
                content,
            )
            for trace in traces:
                data.append({
                    'Trace': int(trace[0]),
                    'Label': trace[1],
                    'Min': int(trace[2]),
                    'Max': int(trace[3]),
                    'Median': float(trace[4]),
                    'Mean': float(trace[5]),
                    'Variance': float(trace[6]),
                    'Max_Square': int(trace[2]) ** 2,
                    'Min_Square': int(trace[3]) ** 2,
                    'Min_Max': (int(trace[2]) - int(trace[3])) ** 2,
                    'CV': ((float(trace[6]) ** 0.5) / float(trace[5])) * 100,
                    'Free_Ele': 1
                })

        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance', 'Min_Max', 'CV']]
        y = df['Label']

        # Step 2: Split data into train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            X, y, test_size=0.2, random_state=i, stratify=y
        )

        # Step 3: Train classifier using KNN
        classifier = KNeighborsClassifier(n_neighbors=10)
        classifier.fit(X_train, y_train)

        # Step 4: Make predictions
        y_pred = classifier.predict(X_test)

        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    # Step 5: Print classification report
    print(classification_report(y_test_full, y_pred_full))


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Evaluate classifiers using trace data.")
    parser.add_argument("file_path", type=str, help="Path to the trace file.")
    args = parser.parse_args()

    print("Evaluating Random Forest Classifier (RFC):")
    eval_rfc(args.file_path)

    print("\nEvaluating k-Nearest Neighbors (KNN):")
    eval_knn(args.file_path)

    print("\nEvaluating Support Vector Machine (SVM):")
    eval_svm(args.file_path)
