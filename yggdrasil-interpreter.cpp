/* This program interprets Yggdrasil, which is a programming language which operates on binary trees. It's esoteric
and hopefully Turing complete, but not necessarily a Turing tarpit. Here is a list of syntax and commands:

TODO: 
- Improve bracket searching algorithm to allow for nested loops and combinators
- Add more bitwise operators / commands

There is a stack, and a tree. Pointer points to the trees root and current uses the * operator to store the node at pointer
which will change throughout the course of the program. Now that I have described the basic syntax, it's time for the list 
of commands:

TREE
< makes the pointer traverse down-left, i.e. replace &current with &(current->left)
> makes the pointer traverse down-right, i.e. replace &current with &(current->right)
^ makes the pointer traverse up, i.e. replace &current with &(current->parent)
% destroys the current node and its children, then traverses up
$ destroys the root (i.e. the entire tree), so not recommended
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
@a is the opposite of [], i.e. if the current node has no children
:c is the opposite of {}, i.e. if the current node's key value is zero
! skips the next command
? goes to another position in the code
s acts like the S combinator in the SKI combinator calculus
K acts like the K combinator in the SKI combinator calculus
O acts like the I combinator in the SKI combinator calculus

STACK AND REGISTERS
N increments the top element of the stack
Q prints the ASCII character associated to the top element of the stack
~ pops the top element of the stack
# pushes the key value of the current node to the stack
A, B, D, F, S, X are general-purpose registers, like AX, BX, CX, DX, ESI and EDI
H, I are stack pointers like ESP and EBP

META
; flips a command and goes to another command (e.g. flipped + is -, etc.)
β copies a command to another place in the code and goes to another third command
τ flips a command and goes to another command if the flipped command is not a certain one
*/

// Basic imports
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

// Very important node struct 
struct node {
  int key_value;
  node *left;
  node *right;
  node *parent;
};

bool operator==(const node& lhs, const node& rhs) {
  return ((lhs->key_value == rhs->key_value) && (lhs->left == lhs->left) && (lhs->right == rhs->right) && (lhs->parent == rhs->parent));
}

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
    bool brk;
    bool quote;
    std::vector<char> digits{"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
    std::vector<char> registers{"A", "B", "D", "F", "S", "X", "H", "I"};
    std::vector<char> positive{">", "^", "+", ".", "v", "V", "w", "W", "[", "]", "{", "}", "?", "K", "O", "N", "Q", 
                               "#", "A", "D", "S", "H", ";", "*"};
    std::map<char,char> flip{{"v", "%"},{"V","$"},{"w","d"},{"W", "C"}, {"d","w"},{"C","W"},{"E","W"},{"!",""},
                             {"[","@"},{"{",":"},{"]","a"},{"}","c"},{"@","["},{":","{"},{"?",""},{"A","D"},
                             {"B","F"},{"S","X"},{"P","G"},{"G","P"},{"H","I"},{"I","H"},{"s",""},{"K","K"},
                             {"O","O"},{"a","]"},{"c","}"},{";",""},{"<",">"},{">","<"},{"^","<"},{"%","v"},
                             {"$","V"},{"N",""},{"Q",""},{"+","-"},{"-","+"},{".",""},{"~","#"},{"#","~"},
                             {"β",""}};
    int AX;
    int BX;
    int CX;
    int DX;
    int ESI;
    int EDI;
    int* EIP;
    int* ESP = &(stack[stack.size()-1]);
    int* EBP = &(stack[0]);
  
    int FindInt(std::string str, int startPos) {
      std::string num;
      int i = startPos;
      while(not NotIn(str[i], digits)) {
        num.append(str[i]);
        i++;
      }
      return stoi(num);
    }

    int FindDigits(std::string str, int i) {
      if(not NotIn(str[i], digits)) {
        a = log10(str[i]+0.01);
        return ceil((a<0)?0:a);
      }
      else return 1;
    }
  
    int InverseFI(std::string str, int endPos) {
      std::string num;
      int i = endPos;
      while(not NotIn(str[i], digits)) {
        num.append(str[i]);
        i--;
      }
      return stoi(num);
    }
  
    std::string FindStr(std::string str, int startPos) {
      std::string quote;
      int i = startPos;
      while(str[i] != '"') {
        quote.append(str[i]);
        i++;
      }
      return quote;
    }
  
    void CheckVar(std::string code, int i) {
      if(not NotIn(code[i+1], digits)) {return FindInt(code,i+1);}
      if(code[i] == '"') {return FindStr(code,i+1)}
      else switch(code[i+1]) {
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
  
    void FindOC(int prevop, int opcode, i) {
      switch(opcode) {
        case 33: SNI = true; break;
        
        case 34:
          
        
        case 35: stack.push(Current->key_value); ESP = &(stack[stack.size()-1]); break;
        case 36: Pointer = &(Current->left); Current = *Pointer; Tree.destroy(); break;
        case 37: Pointer = &(Current->parent); Tree.destroy(Current); Current = *Pointer; break;
        
        case 38:
          x = CheckVar(code,i+1);
          y = CheckVar(code,i+FindDigits(code,x)+3);
          code[y] = code[x];
          pos = FindInt(code,i+FindDigits(code,x)+FindDigits(code,y)+4);
          brk = true;
          break;
        
        case 42:
          x = flip.find({code[CheckVar(code,i+1)]}); 
          if(not NotIn(x,positive)) {
            pos = FindInt(code,i+4);
          }
          break;
        
        case 43: Current->key_value++; break;
        case 45: Current->key_value--; break;
        case 46: std::cout << static_cast<char>(Current.key_value); break;
        
        case 58:
          int startpos = i;
          int endpos = str.find("c");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value == 0) {Run(br);}
        
        case 59: auto fsymb = flip.find({code[InverseFI(code,i-1)]}); code[InverseFI(code,i-1)] = fsymb; pos = CheckVar(code,i); break;
        case 60: Pointer = &(Current->left); Current = *Pointer; break;
        case 62: Pointer = &(Current->right); Current = *Pointer; break;
        case 63: pos = CheckVar(code,i); break;
        
        case 64:
          int startpos = i;
          int endpos = str.find("a");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->left == NULL && Current->right == NULL) {Run(br);}
        
        case 65: AX = CheckVar(code,i); break;
        case 66: BX = CheckVar(code,i); break;
        case 67: Pointer = &(Tree.bottomup_L(CheckVar(code,i))); Current = *Pointer; break;
        case 68: CX = CheckVar(code,i); break;
        case 69: Pointer = &(Tree.bottomup_R(CheckVar(code,i))); Current = *Pointer; break;
        case 70: DX = CheckVar(code,i); break;
        
        case 75: 
          code.erase(code.begin()+i);
          if(code[i+1] != "(") {code.erase(code.begin()+i+1);}
          else {
            int endpos = str.find(")");
            code.erase(code.begin()+endpos+1)
          }
          brk = true; break;
        
        case 78: stack[stack.size()-1] += 1; ESP = &(stack[stack.size()-1]); break;
        case 81: std::cout << static_cast<char>(stack[stack.size()-1]); stack.pop(); ESP = &(stack[stack.size()-1]); break;
        case 83: ESI = CheckVar(code,i); break;
        case 86: Tree.insert(CheckVar(code,i)); break;
        case 87: &(Tree.search(CheckVar(code,i))); Current = *Pointer; break;
        case 88: EDI = CheckVar(code,i); break;
        
        case 91:
          int startpos = i;
          int endpos = str.find("]");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->left != NULL && Current->right != NULL) {Run(br);}
        
        case 94: Pointer = &(Current->parent); Current = *Pointer; break;
        case 115: code.erase(code.begin()+i); brk = true; break;
        case 118: Tree.insert(CheckVar(code,i),Current); break;
        case 119: Pointer = &(Tree.search(CheckVar(code,i),Current)); Current = *Pointer; break;
        case 100: Pointer = &(Tree.bottomup(CheckVar(code,i),Current)); Current = *Pointer; break;
        
        case 123:
          int startpos = i;
          int endpos = str.find("}");
          std::string br;
          for(i = startpos+1; i < endpos; i++) {
            br.append(code[i]);
          }
          if(Current->key_value != 0) {Run(br);}
        
        case 126:
          if(NotIn(static_cast<char>(prevop), registers)) {stack.pop();}
          else {
            switch(prevop) {
              case 65: AX = stack.pop(); break;
              case 66: BX = stack.pop(); break;
              case 68: CX = stack.pop(); break;
              case 70: DX = stack.pop(); break;
              case 71: EBP = &(stack.pop()); break;
              case 80: ESP = &(stack.pop()); break;
              case 83: ESI = stack.pop(); break;
              case 88: EDI = stack.pop(); break;
            }
          }
      }
    }
    
    void BlockExec(std::string code, int i) {
      if(i+1 < code.size()) {EIP = &(code[i+1]);}
      symb = code[i];
      if(SNI == false && quote == false) {FindOC(static_cast<int>(code[i-1]),static_cast<int>(symb),i)}
      if(quote == false) {}
      else {SNI = false;}
      /* switch(symb) {
        case "O":
           copy = code;
           code.erase(code.begin()+i);
           if(code[i+1] != "(") {
            if(code[i+2] != "(") {
              code.erase(code.begin()+i+2);
              if(code[i+3] != "(") {
                code.insert(i+3, "(" + copy[i+2] + copy[i+3]+ ")");
              }
              else {
                startpos = i+3;
                endpos = str.find(")");
                std::string br;
                for(i = startpos+1; i < endpos; i++) {
                  br.append(code[i]);
                }
                code.insert(endpos, "(" + copy[i+2] + br + ")" )
              }
            }
            else {
              // I should probably implement a better bracket searching algorithm
              // so until I've done that I will leave this bit blank
            }
           } Run(code); break; */
   }
   void Run(std::string code, i = 0) {
     for(int j = i; j < code.size(); j++) {
       if (not brk) {BlockExec(code, j);}
       else {j = i;}
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
