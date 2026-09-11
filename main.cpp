cd ..#include <iostream>
#include <cmath>
#include <cstring>
using namespace std;

#define MAX 50

// ------------------------------
// Structure for a Transaction
// ------------------------------
struct Transaction {
    char id[10];
    char dependsOn[10];
    double amount;
    char description[50];
};

// ------------------------------
// MANUAL QUEUE IMPLEMENTATION
// ------------------------------
struct Queue {
    int data[MAX];
    int front, rear;

    void init() {
        front = -1;
        rear = -1;
    }

    bool isEmpty() {
        return (front == -1 || front > rear);
    }

    void enqueue(int val) {
        if (rear == MAX - 1) {
            cout << "Queue Overflow!\n";
            return;
        }
        if (front == -1)
            front = 0;
        data[++rear] = val;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue Underflow!\n";
            return -1;
        }
        return data[front++];
    }
};

// ------------------------------
// GREEDY DETECTION
// ------------------------------
void greedyDetection(Transaction tx[], int n) {
    double mean = 0, std_dev = 0;
    double amounts[MAX];
    int i, j, fraudCount = 0;

    cout << "\n===== [1] GREEDY FRAUD DETECTION =====\n";

    for (i = 0; i < n; i++) {
        amounts[i] = tx[i].amount;
        double sum = 0;
        for (j = 0; j <= i; j++)
            sum += amounts[j];

        mean = sum / (i + 1);
        double variance = 0;
        for (j = 0; j <= i; j++)
            variance += pow(amounts[j] - mean, 2);
        variance /= (i + 1);
        std_dev = sqrt(variance);

        if (i > 0 && (tx[i].amount > mean + 2 * std_dev)) {
            cout << "⚠️  Suspicious transaction (High Amount): "
                 << tx[i].id << " | ₹" << tx[i].amount << endl;
            fraudCount++;
        }
    }

    if (fraudCount == 0)
        cout << "✅ No suspicious transactions found by Greedy detection.\n";
}

// ------------------------------
// PATTERN MATCHING (KMP Algorithm)
// ------------------------------
void computeLPS(double pattern[], int m, int lps[]) {
    int len = 0, i = 1;
    lps[0] = 0;
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMPSearch(double text[], int n, double pattern[], int m) {
    int lps[MAX];
    computeLPS(pattern, m, lps);

    int i = 0, j = 0;
    int found = 0;

    cout << "\n===== [2] PATTERN MATCHING (KMP) =====\n";

    while (i < n) {
        if (pattern[j] == text[i]) {
            i++;
            j++;
        }
        if (j == m) {
            cout << "⚠️  Fraud pattern detected starting at transaction index " << (i - j) << endl;
            found = 1;
            j = lps[j - 1];
        } else if (i < n && pattern[j] != text[i]) {
            if (j != 0)
                j = lps[j - 1];
            else
                i++;
        }
    }

    if (!found)
        cout << "✅ No repeating fraudulent pattern found.\n";
}

// ------------------------------
// SEQUENTIAL TOPOLOGICAL SORT
// ------------------------------
void topologicalSort(Transaction tx[], int n) {
    int indegree[MAX] = {0};
    int adj[MAX][MAX] = {0};
    int i, j;

    // Build dependency graph
    for (i = 0; i < n; i++) {
        if (strlen(tx[i].dependsOn) > 0) {
            for (j = 0; j < n; j++) {
                if (strcmp(tx[j].id, tx[i].dependsOn) == 0) {
                    adj[j][i] = 1;
                    indegree[i]++;
                }
            }
        }
    }

    Queue q;
    q.init();
    for (i = 0; i < n; i++)
        if (indegree[i] == 0)
            q.enqueue(i);

    int count = 0;
    int order[MAX];

    while (!q.isEmpty()) {
        int node = q.dequeue();
        order[count++] = node;

        for (j = 0; j < n; j++) {
            if (adj[node][j] == 1) {
                indegree[j]--;
                if (indegree[j] == 0)
                    q.enqueue(j);
            }
        }
    }

    cout << "\n===== [3] SEQUENTIAL TOPOLOGICAL SORT =====\n";

    if (count != n)
        cout << "⚠️  Cycle detected in transaction dependencies — Invalid or fraudulent flow!\n";
    else {
        cout << "✅ Valid sequential transaction order:\n";
        for (i = 0; i < count; i++)
            cout << tx[order[i]].id << " ";
        cout << endl;
    }
}

// ------------------------------
// MAIN FUNCTION
// ------------------------------
int main() {
    Transaction tx[MAX];
    int n, i;

    cout << "========================================\n";
    cout << "    FRAUD TRANSACTION DETECTION SYSTEM  \n";
    cout << "========================================\n";

    cout << "\nEnter number of transactions: ";
    cin >> n;
    cin.ignore();

    for (i = 0; i < n; i++) {
        cout << "\nEnter details for Transaction " << i + 1 << ":\n";
        cout << "Transaction ID: ";
        cin >> tx[i].id;
        cout << "Depends on (enter '-' if none): ";
        cin >> tx[i].dependsOn;
        if (strcmp(tx[i].dependsOn, "-") == 0)
            strcpy(tx[i].dependsOn, "");
        cout << "Amount (₹): ";
        cin >> tx[i].amount;
        cout << "Description: ";
        cin.ignore();
        cin.getline(tx[i].description, 50);
    }

    // 1. Greedy detection
    greedyDetection(tx, n);

    // 2. Pattern Matching (KMP)
    double text[MAX];
    for (i = 0; i < n; i++)
        text[i] = tx[i].amount;

    int m;
    cout << "\nEnter number of amounts in pattern to search: ";
    cin >> m;

    double pattern[MAX];
    cout << "Enter pattern amounts:\n";
    for (i = 0; i < m; i++)
        cin >> pattern[i];

    KMPSearch(text, n, pattern, m);

    // 3. Topological Sort (dependency check)
    topologicalSort(tx, n);

    cout << "\n===== COMPLEXITY ANALYSIS =====\n";
    cout << "Greedy Detection: O(n)\n";
    cout << "KMP Pattern Matching: O(n + m)\n";
    cout << "Topological Sort: O(V + E)\n";
    cout << "Space Complexity: O(n^2)\n";

    cout << "\n===== CONCLUSION =====\n";
    cout << "This project integrates multiple algorithmic approaches to detect:\n";
    cout << "- Abnormal transaction amounts (Greedy)\n";
    cout << "- Repeated fraud patterns (KMP)\n";
    cout << "- Invalid transaction flows (Topological Sort)\n";
    cout << "Making it efficient and suitable for large-scale fraud analysis systems.\n";

    return 0;
}