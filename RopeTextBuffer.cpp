#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

// Node class
class Node {
public:
    string str;
    double weight;
    Node *left, *right;
    
    Node() {
        left = right = nullptr;
    }
};

// Rope-Like Text Buffer class
class Rope {
public:
    Node *root;
    double chunkSize;
    double ropeSize;
    int leftHeight;
    int rightHeight;
    
    Rope() {
        root = nullptr;
        chunkSize = 10;
        ropeSize = 0;
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
                ropeSize += 20;
                
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
        // Set rope size
        ropeSize += leftString.length() + rightString.length();
    }
    
    void displayChunks(Node *start) {
        // Go inorder but only display nodes with strings (leaf nodes)
        if (start == nullptr) {
			cout <<" Nothing to display" << endl;
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
        return start -> str[index - 1];
    }
	char replaceByte(int index, char replace, Node *start) {
		//similar to searchForByte but it replaces the char instead of returning
        if (start -> weight < index && start -> right != nullptr ) {
            // If index is greater than weight, subtract weight from index and move right
            return replaceByte(index - start -> weight, replace, start -> right);
        }
        if (start -> left != nullptr) {
            // If index is less, move left
            return replaceByte(index, replace, start -> left);
        }
		start->str[index-1] = replace;//(index - 1, 1, replace);
        return start->str[index-1];
    }
    string searchForChunk(int index, Node *start) {
        if (start -> weight < index && start -> right != nullptr ) {
            // If index is greater than weight, subtract weight from index and move right
            return searchForChunk(index - start -> weight, start -> right);
        }
        if (start -> left != nullptr) {
            // If index is less, move left
            return searchForChunk(index, start -> left);
        }
        return start -> str;
    }
	string getString(int begin, int end, Node *start) {
		//Uses searchForByte to find the string between the interval of begin and end
		string result = "";
		for(int i=begin; i <= end; i += 10) {
			result += searchForChunk(i, start);
		}
		return result;
    }
	
	void replaceString(int begin, int end, string insert, Node *start) {
		int j = 0;
		for(int i=begin; i <= end; i++) {
			replaceByte(i, insert[j], start);
            j++;
		}
	}
	void deleteFile(Node *start) {
		//Postorder and then delete
	   if (start == nullptr) {
		   cout << "Nothing To Delete" << endl;
            return;
        }
            if(start -> left != nullptr)
				deleteFile(start -> left);
			
			if(start -> right != nullptr)
				deleteFile(start -> right);

			delete start;
        
	}
	void insertStringAtMiddle(string insert, Node* start) {
        Node *tempSubRoot = new Node;
        tempSubRoot -> right = root -> right;
        
        // Construct tempRoot -> left
        Node *leftSubtree = new Node;
        string leftString = "";
        string rightString = "";
        int currentChunkSize = 0;
        bool leftLeafFilled = false;
        bool rightLeafFilled = false;
        for (int i = 0; i <= insert.length(); i++) {
            if(!leftLeafFilled) {
                // Fill left string
                if (currentChunkSize < chunkSize - 1) {
                    leftString += insert[i];
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and start on right string
                    leftString += insert[i];
                    currentChunkSize = 0;
                    leftLeafFilled = true;
                }
            }
            else if (!rightLeafFilled) {
                // Fill right string
                if (currentChunkSize < chunkSize - 1) {
                    rightString += insert[i];
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and continue to concat with rope
                    rightString += insert[i];
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
                ropeSize += 20;
                
                if (leftSubtree == nullptr) {
                    // Create new rope
                    leftSubtree = tempRoot;
                    leftHeight = 1;
                    rightHeight = 1;
                }
                else {
                    // Concat with current rope
                    // Two cases:
                    // Equal heights (just concatenate)
                    if (leftHeight == rightHeight) {
                        Node *temp = new Node;
                        temp -> left = leftSubtree;
                        temp -> right = tempRoot;
                        temp -> weight = pow(2, leftHeight) * chunkSize; // left weight = (2 ^ height) * chunksize
                        leftSubtree = temp;
                        
                        // Set new heights
                        leftHeight += 1;
                        rightHeight = 2;
                    }
                    // Non equal hieghts (self balance tree while keeping continuity of string)
                    else {
                        Node *temp = new Node;
                        temp -> left = leftSubtree -> right;
                        temp -> right = tempRoot;
                        temp -> weight = pow(2, rightHeight - 1) * chunkSize; // right weight = (2 ^ height - 1) * chunksize
                        leftSubtree -> right = temp;
                        
                        // Set new height
                        rightHeight += 1;
                    }
                }

                // Reset and add current char to left string so process can start again.
                leftLeafFilled = false;
                rightLeafFilled = false;
                leftString = insert[i];
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
        tempRoot -> left = leftSubtree;
        tempRoot -> right = tempBranch;
        tempRoot -> weight = pow(2, leftHeight) * chunkSize;
        leftSubtree = tempRoot;
        // Set new heights
        leftHeight += 1;
        rightHeight = 2;
        // Set rope size
        ropeSize += leftString.length() + rightString.length();
        
        // Concatenate subtrees to form new root
        tempSubRoot -> left = leftSubtree;
        root -> right = tempSubRoot;
	}
    void appendRope(string insert, Node* start) {
        // Initialize local variables
        string leftString = "";
        string rightString = "";
        int currentChunkSize = 0;
        bool leftLeafFilled = false;
        bool rightLeafFilled = false;
        for (int i = 0; i <= insert.length(); i++) {
            if(!leftLeafFilled) {
                // Fill left string
                if (currentChunkSize < chunkSize - 1) {
                    leftString += insert[i];
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and start on right string
                    leftString += insert[i];
                    currentChunkSize = 0;
                    leftLeafFilled = true;
                }
            }
            else if (!rightLeafFilled) {
                // Fill right string
                if (currentChunkSize < chunkSize - 1) {
                    rightString += insert[i];
                    currentChunkSize++;
                }
                else {
                    // Reset local variables and continue to concat with rope
                    rightString += insert[i];
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
                ropeSize += 20;
                
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
                // Reset and add current char to left string so process can start again.
                leftLeafFilled = false;
                rightLeafFilled = false;
                leftString = insert[i];
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
        // Set rope size
        ropeSize += leftString.length() + rightString.length();
    }
	
	void deleteBytes(int bytes,  Node *start) {
        // Going to delete from end
        int indexFromEnd = ropeSize - bytes;
        for (int i = indexFromEnd; i <= ropeSize ; i += chunkSize ) {
            chunkDeleter(i, start);
        }
    }
    void chunkDeleter(int index, Node *start) {
        if (start -> weight < index && start -> right != nullptr ) {
            // If index is greater than weight, subtract weight from index and move right
            return chunkDeleter(index - start -> weight, start -> right);
        }
        if (start -> left != nullptr) {
            // If index is less, move left
            return chunkDeleter(index, start -> left);
        }
        start -> str = "";
        return;
    }
     
	void writeChunks(Node *start, ofstream& outFile) {
        // Go inorder to write strings (leaf nodes) to output file
        if (start == nullptr) {
            return;
        }
        else {
            writeChunks(start -> left, outFile);
            if (!start -> str.empty()) {
               outFile << start -> str;
            }
           writeChunks(start -> right, outFile);
        }
    }
	
	void saveAsFile(Node *start) {
		//Uses writeChunks to append all of the strings in the rope 
        ofstream outFile ("output.txt");
		writeChunks(start, outFile);
		outFile.close();
    }
};

/* -- STRING BENCHMARKS -- */
string loadFileIntoString(string fileName) {
    string str;
    ifstream inFile;
    inFile.open(fileName);
    char character;
    while (inFile.get(character)) {
        str += character;
    }
    return str;
};
void saveStringIntoFile(string str) {
    ofstream outFile ("output.txt");
    for (int i = 0; i <= str.length() - 1; i++) {
        outFile << str[i];
    }
    outFile.close();
};
string stringOfXLength(int x) {
    string str;
    for (int i = 0; i < x; i++) {
        str += "1";
    }
    return str;
}


int main() {
    int k = 200000;
    
    int m = 200000;
    int n = 350000;
    
    int x = 50000;
    
    // Grab file name
    string fileName = "2MB.txt"; // Hard code for testing purposes and comment out cout
    /*
    cout << "What is the name of your text file?" << endl;
    cin >> fileName;
    
    cout << endl;
     */
    
    // Create rope
    Rope rope;
    rope.createRopeWithFile(fileName);
    rope.searchForByte(k, rope.root); // getting kth byte
    rope.getString(m, n, rope.root); // get a string from byte m to n
    //rope.replaceString(m, n, stringOfXLength(n-m), rope.root); // replace string from m to n - ERROR (segmentation fault)
    rope.insertStringAtMiddle(stringOfXLength(x), rope.root); // insert X bytes in middle
    rope.deleteBytes(x, rope.root); // deleting X bytes from end
    rope.saveAsFile(rope.root); // saving text to file
    
    // Create String
    /*
    string test = loadFileIntoString(fileName);
    test[k - 1]; // getting kth byte
    test.substr(m, n - m); // get a string from byte m to n
    //test.replace(m, n - m, stringOfXLength(n-m)); // replace string from m to n
    test.insert(50000000, stringOfXLength(x)); // insert X bytes in middle
    test.erase(test.end() - x, test.end()); // deleting X bytes from end
    saveStringIntoFile(test); // saving text to file
     */
    
    
    return 0;
}
