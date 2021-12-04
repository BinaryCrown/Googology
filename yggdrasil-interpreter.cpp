/* This program interprets Yggdrasil, which is a programming language which operates on binary trees. It's esoteric
and hopefully Turing complete, but not necessarily a Turing tarpit. Here is a list of syntax and commands:

TODO: 
- Allow stack to be popped to a variable, i.e. var = stack.pop() instead of just stack.pop()
- Improve integer searching algorithm to allow for statements such as ?10
- Improve bracket searching algorithm to allow for nested loops

There is a stack, and a tree. Pointer points to the trees root and current uses the * operator to store the node at pointer
which will change throughout the course of the program. Now that I have described the basic syntax, it's time for the list 
of commands:

TREE
< makes the pointer traverse down-left, i.e. replace &current with &(current->left)
> makes the pointer traverse down-right, i.e. replace &current with &(current->right)
^ makes the pointer traverse up, i.e. replace &current with &(current->parent)
% destroys the current node and its children, then traverses up
+ increments the current node's key value
- decrements the current node's key value
. prints the ASCII character associated to the current node's key value
v inserts a node below the current node
V inserts a node below the root
w searches for a node starting at the current node
W searches for a node starting at the root
d searches for a node, bottom-up, starting at the current node
C searches for a node, bottom-up, starting at the leftmost node
E searches for a node, bottom-up, starting at the rightmost node

LOOPS AND FLOW
[] runs the code within the brackets if the current node's children are not null
{} runs the code within the brackets if the current node's key value is not zero
@; is the opposite of [], i.e. the current node has no children
:/ is the opposite of {}, i.e. the current node's key value is zero
! skips the next command
? goes to another position in the code

STACK AND REGISTERS
N increments the top element of the stack
Q prints the ASCII character associated to the top element of the stack
~ pops the top element of the stack
# pushes the key value of the current node to the stack
A, B, D, F, S, X are general-purpose registers, like AX, BX, CX, DX, ESI and EDI
*/

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

bool NotIn(char k, std::vector<char> l) {
  for(int i = 0; i < l.size(); i++) {
    if(l[i] == k) {return false;}
  }
  return true;
}

// The main interpreter class.
class Interpreter() {
  public:
    Stack stack;
    tree Tree;
    std::string code; 
    node* Pointer = &(Tree->root);
    node Current = *Pointer;
    int pos;
    std::vector<char> special{"v", "V", "w", "W", "d", "C", "E", "!", "[", "{", "]", "}", "@", ";", ":", "/", "?", "A", 
                              "B", "D", "F", "S", "X", "P", "G", "H", "I"};
    
    std::vector<char> digits{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int AX;
    int BX;
    int CX;
    int DX;
    int ESI;
    int EDI;
    int* EIP;
    int* ESP = &(stack[stack.size()-1]);
    int* EBP = &(stack[0]);
    void Exec(char symb) {
      switch(symb) {
        case "<": Pointer = &(Current->left); Current = *Pointer;
        case ">": Pointer = &(Current->right); Current = *Pointer;
        case "^": Pointer = &(Current->parent); Current = *Pointer;
        case "%": destroy(Current); Pointer = &(Current->parent); Current = *Pointer;
        case "N": stack[stack.size()-1] += 1;
        case "Q": std::cout << static_cast<char>(stack[stack.size()-1]); stack.pop();
        case "+": Current.key_value++;
        case "-": Current.key_value--;
        case ".": std::cout << static_cast<char>(Current.key_value);
        case "~": stack.pop();
        case "#": stack.push(Current->key_value);
      }
    }
  
    void CheckVar(code) {
      if(not NotIn(code[i+1], digits)) {return stoi(code[i+1])}
      switch(code[i+1]) {
              case "A": return AX;
              case "B": return BX;
              case "D": return CX;
              case "F": return DX;
              case "S": return ESI;
              case "X": return EDI;
              case "P": return ESP;
              case "G": return EBP;
              case "H": return *ESP;
              case "I": return *EBP;
            }
    }
    
    void BlockExec(std::string code, int i) {
      if(i+1 < code.size()) {EIP = &(code[i+1]);}
      symb = code[i];
       if(SNI == false && NotIn(symb, special)) {Exec(symb);}
       if(SNI == true) {SNI = false;}
       switch(symb) {
        case "!": SNI = true; break;
        case "?": pos = CheckVar(code); break;
        case "v": Tree.insert(CheckVar(code),Current); break;
        case "V": Tree.insert(CheckVar(code)); break;
        case "w": Pointer = &(Tree.search(CheckVar(code),Current)); Current = *Pointer; break;}
        case "W": Pointer = &(Tree.search(CheckVar(code))); Current = *Pointer; break;
        case "d": Pointer = &(Tree.bottomup(CheckVar(code),Current)); Current = *Pointer; break;
        case "C": Pointer = &(Tree.bottomup_L(CheckVar(code))); Current = *Pointer; break;
        case "E": Pointer = &(Tree.bottomup_R(CheckVar(code))); Current = *Pointer; break;
        case "A": AX = CheckVar(code); break;
        case "B": BX = CheckVar(code); break;
        case "D": CX = CheckVar(code); break;
        case "F": DX = CheckVar(code); break;
        case "S": ESI = CheckVar(code); break;
        case "X": EDI = CheckVar(code); break;
        case "[":
          int startpos = str.find("[");
          int endpos = str.find("]");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->left != NULL && Current->right != NULL) {Run(br);}
        case "{":
          int startpos = str.find("{");
          int endpos = str.find("}");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value != 0) {Run(br);}
        case "@":
         int startpos = str.find("@");
         int endpos = str.find(";");
         std::string br;
         for(i = startpos+1; i < endpos; i++) {
           br.append(code[i]);
         }
         if(Current->left == NULL && Current->right == NULL) {Run(br);}
        case ":":
          int startpos = str.find(":");
          int endpos = str.find("/");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value == 0) {Run(br);}
       }
   }
   void Run(std::string code, i = 0) {
     for(int j = i; j < code.size(); j++) {
       BlockExec(code, j);
     }
     if(pos != NULL) {
       Run(code, pos);
     }
   }
  
   void Error(std::string message) {
     std::cerr << "Error: " + message + "\n";
   }
}

int main() {
  return 0;
}
