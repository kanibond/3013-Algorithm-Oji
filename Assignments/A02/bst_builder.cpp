//Name: Kanayo Oji
//Instructor: Prof. Griffin
//Class: 3013 - Algorithm
//Project: Assignment 2
//Description: This program gets data from a textfile, sorts it 
//            and creates a complete and balanced BST. 
//Output: A graphviz data that could be used to verify/check if tree
//        is balanced.


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>


//http://www.webgraphviz.com/

using namespace std;

struct node
{
	int data;
	node *left;
	node *right;
	node()
	{
		data = -1;
		left = NULL;
		right = NULL;
	}
	node(int x)
	{
		data = x;
		left = NULL;
		right = NULL;
	}
};

class BSTree
{
private:
	node *root;

	int count(node *root)
	{
		if (!root)
		{
			return 0;
		}
		else
		{
			return 1 + count(root->left) + count(root->right);
		}
	}

	void insert(node *&root, node *&temp)
	{
		if (!root)
		{
			root = temp;
		}
		else
		{
			if (temp->data < root->data)
			{
				insert(root->left, temp);
			}
			else
			{
				insert(root->right, temp);
			}
		}
	}

	void print_node(node *n, string label = "")
	{
		if (label != "")
		{
			cout << "[" << label << "]";
		}
		cout << "[[" << n << "][" << n->data << "]]\n";
		if (n->left)
		{
			cout << "\t|-->[L][[" << n->left << "][" << n->left->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[L][null]\n";
		}
		if (n->right)
		{
			cout << "\t\\-->[R][[" << n->right << "][" << n->right->data << "]]\n";
		}
		else
		{
			cout << "\t\\-->[R][null]\n";
		}
	}

	/**
	 * type = ['predecessor','successor']
	 */
	node *minValueNode(node *root)
	{
		node *current = root;

		if (root->right)
		{
			current = root->right;
			while (current->left != NULL)
			{
				current = current->left;
			}
		}
		else if (root->left)
		{
			current = root->left;
			while (current->right != NULL)
			{
				current = current->right;
			}
		}

		return current;
	}

	int height(node *root)
	{
		if (!root)
		{
			return 0;
		}
		else
		{
			int left = height(root->left);
			int right = height(root->right);
			if (left > right)
			{
				return left + 1;
			}
			else
			{
				return right + 1;
			}
		}
	}


	/* Print nodes at a given level */
	void printGivenLevel(node *root, int level)
	{
		if (root == NULL)
			return;
		if (level == 1)
		{
			print_node(root);
		}
		else if (level > 1)
		{
			printGivenLevel(root->left, level - 1);
			printGivenLevel(root->right, level - 1);
		}
	}
	//************************************************************************
	// Method to help create GraphViz code so the expression tree can
	// be visualized. This method prints out all the unique node id's
	// by traversing the tree.
	// Recivies a node pointer to root and performs a simple recursive
	// tree traversal.
	//************************************************************************
	void GraphVizGetIds(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizGetIds(nodePtr->left, VizOut);
			VizOut << "node" << nodePtr->data
				<< "[label=\"" << nodePtr->data << "\\n"
				//<<"Add:"<<nodePtr<<"\\n"
				//<<"Par:"<<nodePtr->parent<<"\\n"
				//<<"Rt:"<<nodePtr->right<<"\\n"
				//<<"Lt:"<<nodePtr->left<<"\\n"
				<< "\"]" << endl;
			if (!nodePtr->left)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
			GraphVizGetIds(nodePtr->right, VizOut);
			if (!nodePtr->right)
			{
				NullCount++;
				VizOut << "nnode" << NullCount << "[label=\"X\",shape=point,width=.15]" << endl;
			}
		}
	}

	//************************************************************************
	// This method is partnered with the above method, but on this pass it
	// writes out the actual data from each node.
	// Don't worry about what this method and the above method do, just
	// use the output as your told:)
	//************************************************************************
	void GraphVizMakeConnections(node *nodePtr, ofstream &VizOut)
	{
		static int NullCount = 0;
		if (nodePtr)
		{
			GraphVizMakeConnections(nodePtr->left, VizOut);
			if (nodePtr->left)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->left->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			if (nodePtr->right)
				VizOut << "node" << nodePtr->data << "->"
				<< "node" << nodePtr->right->data << endl;
			else
			{
				NullCount++;
				VizOut << "node" << nodePtr->data << "->"
					<< "nnode" << NullCount << endl;
			}

			GraphVizMakeConnections(nodePtr->right, VizOut);
		}
	}

public:
	BSTree()
	{
		root = NULL;
	}
	~BSTree()
	{
	}

	int count()
	{
		return count(root);
	}

	void insert(int x)
	{
		node *temp = new node(x);
		insert(root, temp);
	}

	void minValue()
	{
		print_node(minValueNode(root), "minVal");
	}

	int height()
	{

		return height(root);
	}

	int top()
	{
		if (root)
			return root->data;
		else
			return 0;
	}


	/* Function to line by line print level order traversal a tree*/
	void printLevelOrder()
	{
		cout << "Begin Level Order===================\n";
		int h = height(root);
		int i;
		for (i = 1; i <= h; i++)
		{
			printGivenLevel(root, i);
			cout << "\n";
		}
		cout << "End Level Order===================\n";
	}

	//************************************************************************
	// Recieves a filename to place the GraphViz data into.
	// It then calls the above two graphviz methods to create a data file
	// that can be used to visualize your expression tree.
	//************************************************************************
	void GraphVizOut(string filename)
	{
		ofstream VizOut;
		VizOut.open(filename);
		VizOut << "Digraph G {\n";
		GraphVizGetIds(root, VizOut);
		GraphVizMakeConnections(root, VizOut);
		VizOut << "}\n";
		VizOut.close();
	}
};
// This function creates a balanced BST from sorted array
void balanceTree(vector <int> BsTree, BSTree &BST, int front, int back)
{
	// base case
	if (front <= back)
	{
	// find middle element 
		int mid = (front + back) / 2;
		BST.insert(BsTree[mid]);
		balanceTree(BsTree, BST, front, mid - 1);
		balanceTree(BsTree, BST, mid + 1, back);

	}	
}

int main()
{
	vector <int> BsTree;
	BSTree B;
	int bstsize; // stores size of the vector
	ifstream infile; //Input file stream object

	infile.open("input.dat"); //Opening the file

	int key; //data to be read from textfile
	//Read in textfile
	while (infile >> key)
	{
		BsTree.push_back(key);
	}
	//sort the vector
	sort(BsTree.begin(), BsTree.end());
	bstsize = BsTree.size(); // assign size of vector to the variable bstsize
	
	//Print out number from file into the vector
	for (int i = 0; i < bstsize; i++) {
		//B.insert(BsTree[i]);
		cout << BsTree[i] << " ";
	}
	//function call to balance tree
	balanceTree(BsTree, B, 0, bstsize - 1);
	infile.close(); //Close the file	
	cin.get(); //Keep program open until "enter" is pressed
	B.printLevelOrder();
	B.GraphVizOut("before.txt");

	return 0;
}
