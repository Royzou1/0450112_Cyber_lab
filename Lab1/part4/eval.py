import json
import numpy as np

from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report
from sklearn.model_selection import train_test_split


def eval_RFC_good():
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open('../analysed.out', 'r') as file:
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
                    'Variance': float(trace[6])
                })

        # Convert to DataFrame
        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Variance']]

        # Generate additional features (linear and non-linear)
        X = X.copy()  # Ensure X is a deep copy of the original DataFrame
        X['Range'] = X['Max'] - X['Min']
        X['Log_Variance'] = np.log1p(X['Variance'])
        X['Min*Max'] = X['Min'] * X['Max']
        X['Median^2'] = X['Median'] ** 2

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


def eval_RFC():
    y_pred_full, y_test_full = [], []

    for i in range(10):
        # Step 1: Load data from traces file
        data = []
        with open('../analysed.out', 'r') as file:
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
                    'Variance': float(trace[6])
                })

        # Convert to DataFrame
        df = pd.DataFrame(data)
        X = df[['Min', 'Max', 'Median', 'Mean', 'Variance']]

        # Generate additional features (linear and non-linear)
        X = X.copy()  # Ensure X is a deep copy of the original DataFrame
        X['Range'] = X['Max'] - X['Min']
        X['Mean/Median'] = X['Mean'] / X['Median']
        X['Log_Variance'] = np.log1p(X['Variance'])
        X['Min*Max'] = X['Min'] * X['Max']
        X['Median^2'] = X['Median'] ** 2

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


def eval():
    y_pred_full, y_test_full = [], []

    # Re-train 10 times in order to reduce effects of randomness
    for i in range(10):
        ### TODO: Exercise 4-1
        ### 1. Load data from traces file
        ### 2. Split data into X_train, X_test, y_train, y_test with train_test_split
        ### 3. Train classifier with X_train and y_train
        ### 4. Use classifier to make predictions on X_test. Save the result to a variable called y_pred

        # Do not modify the next two lines
        y_test_full.extend(y_test)
        y_pred_full.extend(y_pred)

    ### TODO: Exercise 4-1 (continued)
    ### 5. Print classification report using y_test_full and y_pred_full

if __name__ == "__main__":
    eval_RFC_good()
    eval_RFC()
