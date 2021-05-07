#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

// Node class
class Node {
public:
    string str;
    int weight;
    Node *left, *right;
    
    Node() {
        left = right = nullptr;
    }
};

// Rope-Like Text Buffer class
class Rope {
public:
    Node *root;
    int chunkSize;
    int leftHeight;
    int rightHeight;
    
    Rope() {
        root = nullptr;
        chunkSize = 10;
        leftHeight = 0;
        rightHeight = 0;
    }
    
    void createRopeWithFile(string fileName) {
        ifstream inFile;
        inFile.open(fileName);
        char character;
        // Initialize local variables
        string leftString = "";
        string rightString = "";
        int currentChunkSize = 0;
        bool leftLeafFilled = false;
        bool rightLeafFilled = false;
        while (inFile.get(character)) {
            if(!leftLeafFilled) {
                // Fill left string
                if (currentChunkSize < chunkSize - 1) {
                    leftString += character;
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and start on right string
                    leftString += character;
                    currentChunkSize = 0;
                    leftLeafFilled = true;
                }
            }
            else if (!rightLeafFilled) {
                // Fill right string
                if (currentChunkSize < chunkSize - 1) {
                    rightString += character;
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and continue to concat with rope
                    rightString += character;
                    currentChunkSize = 0;
                    rightLeafFilled = true;
                }
            }
            else {
                // Create new leaf structure
                Node *left = new Node;
                Node *right = new Node;
                // Input vals
                left -> str = leftString;
                left -> weight = leftString.length();
                right -> str = rightString;
                right -> weight = rightString.length();
                // Construct root
                Node *tempRoot = new Node;
                tempRoot -> left = left;
                tempRoot -> right = right;
                tempRoot -> weight = left -> weight;
                
                if (root == nullptr) {
                    // Create new rope
                    root = tempRoot;
                    leftHeight = 1;
                    rightHeight = 1;
                }
                else {
                    // Concat with current rope
                    // Two cases:
                    // Equal heights (just concatenate)
                    if (leftHeight == rightHeight) {
                        Node *temp = new Node;
                        temp -> left = root;
                        temp -> right = tempRoot;
                        temp -> weight = pow(2, leftHeight) * chunkSize; // left weight = (2 ^ height) * chunksize
                        root = temp;
                        
                        // Set new heights
                        leftHeight += 1;
                        rightHeight = 2;
                    }
                    // Non equal hieghts (self balance tree while keeping continuity of string)
                    else {
                        Node *temp = new Node;
                        temp -> left = root -> right;
                        temp -> right = tempRoot;
                        temp -> weight = pow(2, rightHeight - 1) * chunkSize; // right weight = (2 ^ height - 1) * chunksize
                        root -> right = temp;
                        
                        // Set new height
                        rightHeight += 1;
                    }
                }

                // Reset and add current char to left string so process can start again.
                leftLeafFilled = false;
                rightLeafFilled = false;
                leftString = character;
                rightString.clear();
                currentChunkSize++;
                
            }
        }
        // Anything left over needs to be appended to the rope as well.
        Node *tempRoot = new Node;
        Node *tempBranch = new Node;
        Node *leafLeft = new Node;
        Node *leafRight = new Node;
        // Input vals
        leafLeft -> str = leftString;
        leafLeft -> weight = leftString.length();
        leafRight -> str = rightString;
        leafRight -> weight = rightString.length();
        tempBranch -> left = leafLeft;
        tempBranch -> right = leafRight;
        tempBranch -> weight = leafLeft -> weight;
        // Concat
        tempRoot -> left = root;
        tempRoot -> right = tempBranch;
        tempRoot -> weight = pow(2, leftHeight) * chunkSize;
        root = tempRoot;
        // Set new heights
        leftHeight += 1;
        rightHeight = 2;
    }
    
    void displayChunks(Node *start) {
        // Go inorder but only display nodes with strings (leaf nodes)
        if (start == nullptr) {
            return;
        }
        else {
            displayChunks(start -> left);
            if (!start -> str.empty()) {
                cout << start -> str << endl;
            }
            displayChunks(start -> right);
        }
    }
    
    char searchForByte(int index, Node *start) {
        if (start -> weight < index && start -> right != nullptr ) {
            // If index is greater than weight, subtract weight from index and move right
            return searchForByte(index - start -> weight, start -> right);
        }
        if (start -> left != nullptr) {
            // If index is less, move left
            return searchForByte(index, start -> left);
        }
        return start -> str.at(index - 1);
    }
};


int main() {
    // Grab file name
    string fileName = "";
    cout << "What is the name of your text file?" << endl;
    cin >> fileName;
    
    cout << endl;
    
    // Create rope
    Rope rope;
    rope.createRopeWithFile(fileName);
    
    // rope.displayChunks(rope.root);
    
    return 0;
}
