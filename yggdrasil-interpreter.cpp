// This program interprets Yggdrasil, which is a programming language which operates on binary trees.

// Basic imports
#include <iostream>
#include <vector>
#include <string>

// Very important node struct 
struct node {
  int key_value;
  node *left;
  node *right;
  node *parent;
};

// Binary tree class. Methods such as search will be defined outside (i.e. using the :: operator)
class tree {
    public:
        node root;
        tree() {
          root = NULL;
        }
        ~tree() {
          destroy();
        }
        void insert(int key) {
          if(root != NULL) {
            insert(key, root);
          }
          else {
            root = new node;
            root->key_value = key;
            root->left = NULL;
            root->right = NULL;
            root->parent = NULL;
          }
        }
        void insert(int key, node *leaf);
        node *search(int key);
        void destroy() {
          destroy(root);
        }
 
    private:
        void destroy(node *leaf) {
          if(leaf != NULL) {
            destroy(leaf->left);
            destroy(leaf->right);
            delete leaf;
          }
        }
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        node *root;
};

// Searches for a node with key value equal to key, starting at leaf
node *tree::search(int key, node *leaf) {
  if(leaf != NULL) {
    if(key == leaf->key_value) {
      return leaf;
    }
    if(key < leaf->key_value) {
      return search(key, leaf->left);
    }
    else {
      return search(key, leaf->right);
    }
  }
  else return NULL;
}

// Bottom-up searching method which is slightly harder to implement
node *tree::bottomup(int key, node *leaf) {
  if(leaf != NULL) {
    if(key == leaf->key_value) {
      return leaf;
    }
    if(key < leaf->key_value) {
      if(leaf->parent->left == leaf) {
        return leaf->parent;
      }
      else {
        return leaf->parent->left;
      }
    }
    else {
      if(leaf->parent->right == leaf) {
        return leaf->parent;
      }
      else {
        return leaf->parent->right;
      }
    }
  }
  else return NULL;
}

// Inserts a node into leaf
void tree::insert(int key, node *leaf) {
  if(key< leaf->key_value) {
    if(leaf->left!=NULL) {
     insert(key, leaf->left);
    }
    else {
      leaf->left = new node;
      leaf->left->key_value = key;
      leaf->left->left = NULL;
      leaf->left->right = NULL;
      leaf->left->parent = leaf;
    }  
  }
  else if(key >= leaf->key_value) {
    if(leaf->right!=NULL) {
      insert(key, leaf->right);
    }
    else {
      leaf->right = new node;
      leaf->right->key_value = key;
      leaf->right->left = NULL;
      leaf->right->right = NULL;
      leaf->right->parent = leaf;
    }
  }
}

// Inserts into tree (i.e. into root)
void tree::insert(int key) {
  if(root!=NULL) {
    insert(key, root);
  }
  else {
    root = new node;
    root->key_value = key;
    root->left = NULL;
    root->right = NULL;
    root->parent = NULL;
  }
}

// Searches through tree (starting at root)
node *tree::search(int key) {
  return search(key, root);
}

// Searches through tree (starting at leftmost node)
node *tree::bottomup_L(int key) {
  point = root;
  while(point->left != NULL) {
    point = point->left;
  }
  return bottomup(key, point);
}

node *tree::bottomup_R(int key) {
  point = root;
  while(point->right != NULL) {
    point = point->right;
  }
  return bottomup(key, point);
}

// Short stack class with methods to push and pop
class Stack {
  public:
    std::vector<int> stack;
    int pop(){
      k = stack[stack.size()-1];
      stack.pop_back();
      return k;
    }
  
    void push(vector<int> t){
      for(int i = 0; i < t.size(); i++) {
        stack.push_back(t[i]);
      }
    }
}

// The main interpreter class.
class Interpreter() {
  public:
    Stack stack;
    tree Tree;
    std::string code; 
    node* Pointer = &(Tree->root);
    node Current = *Pointer;
    
    void Exec(char symb) {
      if(symb == "<") {Pointer = &(Current->left);}
      if(symb == ">") {Pointer = &(Current->right);}
      if(symb == "^") {Pointer = &(Current->parent);}
      if(symb == "%") {destroy(Current); Pointer = &(Current->parent);}
      if(symb == "N") {stack[stack.size()-1] += 1;}
      if(symb == "Q") {std::cout << static_cast<char>(stack[stack.size()-1]); stack.pop();}
      if(symb == "+") {Current.key_value++;}
      if(symb == "-") {Current.key_value--;}
      if(symb == ".") {std::cout << static_cast<char>(Current.key_value);}
      if(symb == "~") {stack.pop();}
      if(symb == "#") {stack.push(Current->key_value);}
    }
  
    void Run(std::string code) {
      for(int i = 0; i < code.size(); i++) {
        symb = code[i];
        if(SNI == false && symb != "v" && symb != "V" && symb != "W" && symb != "s" && symb != "!" && symb != "[" && symb != "{" && symb != "]" && symb != "}" && symb != "@" && symb != ";" && symb != ":" && symb != "/") {Exec(symb);}
        if(SNI == true) {SNI = false; continue;}
        if(symb == "!") {SNI = true; continue;}
        if(symb == "[") {
          int startpos = str.find("[");
          int endpos = str.find("]");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->left != NULL && Current->right != NULL) {Run(br);}
        }
        if(symb == "{") {
          int startpos = str.find("{");
          int endpos = str.find("}");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value != 0) {Run(br);}
        }
        if(symb == "@") {
          int startpos = str.find("@");
          int endpos = str.find(";");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->left == NULL && Current->right == NULL) {Run(br);}
        }
        if(symb == ":") {
          int startpos = str.find(":");
          int endpos = str.find("/");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value == 0) {Run(br);}
        }
        if(symb == "v") {
          Tree.insert(stoi(code[i+1]),Current);
        }
        if(symb == "V") {
          Tree.insert(stoi(code[i+1]),Tree->root);
        }
        if(symb == "w") {
          Pointer = &(Tree.search(stoi(code[i+1]),Current));
        }
        if(symb == "W") {
          Pointer = &(Tree.search(stoi(code[i+1])));
        }
        if(symb == "d") {
          Pointer = &(Tree.bottomup(stoi(code[i+1]),Current));
        }
        if(symb == "C") {
          Pointer = &(Tree.bottomup_L(stoi(code[i+1])));
        }
        if(symb == "E") {
          Pointer = &(Tree.bottomup_R(stoi(code[i+1])));
        }
      }
    }
  
    void Error(std::string message) {
      std::cerr << "Error: " + message + "\n";
    }
}

int main() {
  return 0;
}
