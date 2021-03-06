/*
	animal.cpp

	author: L. Henke and C. Painter-Wakefield
	date: 04 November 2019

	Animal/20 questions program for CSCI 262, starter code.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <queue>
#include <utility>

using namespace std;

class node {
public:
    node(){
        data = "";
        left = nullptr;
        right = nullptr;
    }
    explicit node(string d){
        data = std::move(d);
        left = nullptr;
        right = nullptr;
    }
    string data;
    node* left;
    node* right;
};

void play_game(node*);
node* read_game_tree();
void write_game_tree(node*);
void delete_game_tree(node*);
void read_preorder(node* &cur, ifstream &fin);
void write_preorder(node* &root, string &s);
void expand_game_tree(node* root, const vector<bool>& answers);
string trim(string s);
bool get_yesno(string msg);

/**
 * Handles showing the main menu/basic UI
 */
int main() {
	node* root = read_game_tree();
	if (root == nullptr) return -1;

	while (true) {
		string tmp;
		int choice;

		cout << "Welcome to 20 questions!" << endl;
		cout << "  1) Play the game" << endl;
		cout << "  2) Save the game file" << endl;
		cout << "  3) Quit" << endl;
		cout << "Please make your selection: ";
		getline(cin, tmp);
		choice = atoi(tmp.c_str());

		switch (choice) {
    		case 1:
    			play_game(root);
    			break;
    		case 2:
    			write_game_tree(root);
    			break;
    		case 3:
    			break;
    		default:
    			cout << "Sorry, I don't understand." << endl << endl;
		}
		if (choice == 3) break;
	}
	delete_game_tree(root);
	return 0;
}

/**
 * Sets up the recursive call to the read_preorder
 * @return root of the tree
 */
node* read_game_tree() {
    ifstream fin("animal_game_tree.txt");
    string s;
    node* head = new node;
    getline(fin,s);
    head->data = s.substr(3);
    read_preorder(head, fin);
    return head;
}

void read_preorder(node* &cur, ifstream &fin){
    if(fin.eof()){
        return;
    }
    string s;
    getline(fin,s);
    if(s.empty()) return;
    if(!cur->left){
        cur->left = new node(s.substr(3));
        if(s[1] == 'Q'){
            read_preorder(cur->left, fin);
        }
    }
    getline(fin,s);
    if(s.empty()) return;
    if(!cur->right){
        cur->right = new node(s.substr(3));
        if(s[1] == 'Q'){
            read_preorder(cur->right, fin);
        }
    }
}


/**
 * Plays the game
 * @param root Root of the game tree
 */
void play_game(node* root) {
    node head = *root;
    vector<bool> answers{};
    while(root != nullptr){
        try {
            if(get_yesno(root->data + "(y/n)")){
                answers.push_back(true);
                root = root->left;
                if(root == nullptr){
                    cout << "YAY! I won!!" << endl;
                }
            } else {
                answers.push_back(false);
                if(root->right == nullptr){
                    cout << "I lost, boo!" << endl;
                    expand_game_tree(&head,answers);
                    return;
                } else {
                    root = root->right;
                }
            }
        } catch (runtime_error& e){
            cout << e.what();
            continue;
        }
    }
}

void expand_game_tree(node* root, const vector<bool>& answers){
    if(!get_yesno("would you like to expand the game tree? (y/n)")){
        return;
    }
    cout << "I asked the following:\n";
    int i = 0;
    for (;i < answers.size() - 2; i++) {
        if(answers[i]){
            cout << root->data << " YES\n";
            root = root->left;
        } else {
            cout << root->data << " NO\n";
            root = root->right;
        }
    }
    cout << root->data << (answers[i + 1] ? " YES\n" :" NO\n");
    cout << root->left->data << " NO\n";
    cout << "Enter a new animal in the form of a question,\n e.g., 'Is it a whale?':";
    string left;
    getline(cin,left);
    cout << "Now enter a question for which the answer is 'yes' for your new\n animal, and which does not contradict your previous answers:";
    string parentQ;
    getline(cin,parentQ);
    node temp = *root->right;
    root->right = new node(parentQ);
    root->right->left = new node(left);
    root->right->right = new node(temp.data);
}

/**
 * Writes the game tree, sets up a recursive call to write_preorder();
 * @param root The root of the tree
 */
void write_game_tree(node* root) {
    ofstream file;
    file.open("animal_game_tree.txt");
    cout << "starting to write tree to file...\n";
    string s{};
    write_preorder(root,s);
    s = s.substr(0,s.size() - 1);
    file << s;
    file.close();
    cout << "file has been written!\n";
}
void write_preorder(node* &root, string &s){
    if(root == nullptr){
        return;
    }
    string res = "#";
    if(root->right || root->left){
        res += "Q ";
    } else {
        res += "A ";
    }
    res += root->data;
    res += '\n';
    s += res;
    write_preorder(root->left,s);
    write_preorder(root->right,s);
}
/**
 * Deletes the game tree
 * @param root Root of the game tree
 */
void delete_game_tree(node* root) {
    // Optional. Do a post-order deletion of the game tree.
    // This isn't strictly needed as the program exits after this is called,
    // which frees up all the memory anyway.
}


bool get_yesno(string msg) {
    while (true) {
        string input;

        cout << msg << endl;
        getline(cin, input);

        input = trim(input);
        for (int i = 0; i < input.size(); i++) {
            input[i] = tolower(input[i]);
        }

        if (input == "y" || input == "yes") return true;
        if (input == "n" || input == "no") return false;

        cout << "I didn't understand that. ";
        cout << "Please enter y(es) or n(o).";
        cout << endl;
    }
}

string trim(string s) {
    int a, b;

    for (a = 0; a < s.size() && isspace(s[a]); a++);
    for (b = s.size() - 1; b >= a && isspace(s[b]); b--);

    return s.substr(a, b - a + 1);
}