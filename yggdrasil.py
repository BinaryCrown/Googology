import math

class Node:
    def __init__(self,key,index):
        self.key = key
        self.index = index
        self.left = 2*index + 1
        self.right = 2*index + 2
        self.parent = math.floor((index-1)/2)

def destroy(leaf,tree):
    if leaf != None:
        tree[leaf.left] = destroy(tree[leaf.left],tree)
        tree[leaf.right] = destroy(tree[leaf.right],tree)
        del tree[leaf]
    return tree

def insertleaf(key,leaf,tree):
    if key < leaf.key:
        if leaf.left >= len(tree):
            tree = insertleaf(key, tree[leaf.left], tree)
        else:
            k = len(tree)
            for i in range(k-leaf.left+1):
                tree.append(Node(None,k+i))
            tree[leaf.left] = Node(key)
    elif key >= leaf.key:
        if leaf.right >= len(tree):
            tree = insertleaf(key, tree[leaf.right], tree)
        else:
            k = len(tree)
            for i in range(k-leaf.right):
                tree.append(Node(None,k+i))
            tree[leaf.right] = Node(key)
    return tree

def searchleaf(key,leaf,tree):
    if leaf != None:
        if key == leaf.key: return leaf
        if key < leaf.key: return searchleaf(key, tree[leaf.left], tree)
        else: return searchleaf(key, tree[leaf.right], tree)

def searchleaf_bottomup(key,leaf,tree):
    if leaf != None:
        if key == leaf.key: return leaf
        if key < leaf.key:
            if tree[tree[leaf.parent].left] == leaf: return tree[leaf.parent]
            else: return tree[tree[leaf.parent].left]
        else:
            if tree[tree[leaf.parent].right] == leaf: return tree[leaf.parent]
            else: return tree[tree[leaf.parent].right]

class Tree:
    def __init__(self,root):
        self.tree = [root]
    
    def __del__(self):
        self.tree = destroy(self.root,self.tree)
    
    def insert(self,key):
        if len(self.tree) != 0:
            self.tree = insertleaf(key,self.root,self.tree)
        else:
            self.tree.append(Node(key,0))
    
    def search(self,key):
        return searchleaf(key,self.tree[0],self.tree)

    def bottomup_L(self,key):
        point = self.tree[0]
        while self.tree[point.left] != None:
            point = self.tree[point.left]
        return searchleaf_bottomup(key, point, self.tree)
    
    def bottomup_R(self,key):
        point = self.tree[0]
        while self.tree[point.right] != None:
            point = self.tree[point.right]
        return searchleaf_bottomup(key, point, self.tree)

class Stack:
    def __init__(self,stack):
        self.stack = stack
    
    def pop(self):
        return self.stack.pop()
    
    def push(self,nums):
        if type(nums) is int: self.stack.append(nums)
        else:
            for i in nums:
                self.stack.append(i)

def IsIn(k, l):
    for i in l:
        if i == k: return True
    return False

class Interpreter:
    def __init__(self,options):
        self.stack = options["stack"]
        self.tree = options["tree"]
        self.code = options["code"]
        self.pointer = 0
        self.current = self.tree.tree[self.pointer]
        self.position = 0
        self.brk = False
        self.quote = False
        self.digits = [str(i) for i in range(10)]
        self.registers = ["A", "B", "D", "F", "S", "X", "H", "I"]
        self.positiive = [">", "^", "+", ".", "v", "V", "w", "W", "[", "]", "{", "}", "?", "K", "O", "N", "Q", "#", "A", "D", "S", "H", ";", "*"]
        self.flip = {
            "v": "%",
            "V": "$",
            "w": "d",
            "W": "C",
            "d": "w",
            "C": "W",
            "E": "W",
            "!": "",
            "[": "@",
            "{": ":",
            "]": "a",
            "}": "c",
            "@": "[",
            ":": "{",
            "?": "",
            "A": "D",
            "B": "F",
            "S": "X",
            "P": "G",
            "G": "P",
            "H": "I",
            "I": "H",
            "s": "",
            "K": "K",
            "O": "O",
            "a": "]",
            "c": "}",
            ";": "",
            "<": ">",
            ">": "<",
            "^": "<",
            "%": "v",
            "$": "V",
            "N": "",
            "Q": "",
            "+": "-",
            "-": "+",
            ".": "",
            "~": "#",
            "#": "~",
            "&": "",
            "*": ""
        }
        self.OC = {
            "[": "]",
            "{": "}",
            "@": "a",
            ":": "c"
        }
        self.AX = 0
        self.BX = 0
        self.CX = 0
        self.DX = 0
        self.ESI = 0
        self.EDI = 0
        self.EIP = 0
        self.ESP = len(self.stack.stack)-1
        self.EBP = 0

        def Error(message):
            print("Error: " + message)
        
        def FMB(open,close,startPos):
            endPos = startPos
            i = 1
            while i > 0:
                endPos += 1
                b = self.code[endPos]
                if IsIn(b, open): i += 1
                if IsIn(b, close): i -= 1
            
            if self.code[endPos] == self.OC[self.code[startPos]]: return endPos
            else: Error("Unbalanced brackets.")
        
        def FI(st,startPos):
            num = ""
            i = startPos
            while IsIn(st[i], self.digits):
                num += st[i]
                i += 1
            return num
        
        def FD(st,i):
            if IsIn(st[i],self.digits):
                a = math.log10(int(str[i]) + 0.01)
                return math.ceil(0 if a < 0 else a)
        
        def IFI(st,endPos):
            num = ""
            i = endPos
            while IsIn(st[i], self.digits):
                num += st[i]
                i -= 1
            return num
        
        def FS(st,startPos):
            quote = ""
            i = startPos
            while st[i] != '"':
                quote += st[i]
                i += 1
            return quote
        
        def CV(code, i):
            if IsIn(code[i+1], self.digits): return FI(code,i+1)
            elif code[i] == '"': return FS(code, i+1)
            elif code[i+1] == "A": return self.AX
            elif code[i+1] == "B": return self.BX
            elif code[i+1] == "D": return self.CX
            elif code[i+1] == "F": return self.DX
            elif code[i+1] == "J": return i
            elif code[i+1] == "K": return self.code[self.EIP]
            elif code[i+1] == "c": return self.EIP
            elif code[i+1] == "S": return self.ESI
            elif code[i+1] == "X": return self.EDI
            elif code[i+1] == "P": return self.ESP
            elif code[i+1] == "G": return self.EBP
            elif code[i+1] == "H": return self.stack[self.ESP]
            elif code[i+1] == "I": return self.stack[self.EBP]
        
        def Run(st, i = 0):
            for j in range(i,len(st)):
                if not self.brk: BlockExec(st, j)
                if self.position != None:
                    Run(st,self.position)
                else: j = i
        
        def FOC(self, prevop, opcode, i):
            match opcode:
                case 33:
                    self.SNI = True
            
                case 35:
                    self.stack.push(self.current.key)
                    self.ESP = len(self.stack.stack) - 1
                
                case 36:
                    self.pointer = self.current.left
                    self.current = self.tree.tree[self.pointer]
                    del self.tree
                
                case 37:
                    self.pointer = self.current.parent
                    self.tree = destroy(self.curent, self.tree.tree)
                    self.current = self.tree.tree[self.pointer]
                
                case 38:
                    x = CV(self.code, i+1)
                    y = CV(self.code, i+FD(self.code,x)+3)
                    self.code[y] = self.code[x]
                    self.position = FI(self.code, i+FD(self.code,x)+FD(self.code,y)+4)
                    self.brk = True
                
                case 42:
                    x = self.flip[self.code[CV(self.code,i+1)]]
                    if IsIn(x, self.positive):
                        self.pos = FI(self.code,i+4)
                
                case 43:
                    self.current.key += 1
                
                case 45:
                    self.current.key -= 1
                
                case 46:
                    print(chr(self.current.key))
                
                case 58:
                    startpos = i
                    endpos = FMB(["[", "{", "@", ":"], ["]", "}", "a", "c"], startpos)
                    br = ""
                    for j in range(startpos+1, endpos):
                        br += self.code[j]
                    if self.current.key == 0: Run(br)
                
                case 59:
                    fsymb = self.flip[self.code[IFI(self.code,i-1)]]
                    self.code[IFI(self.code,i-1)] = fsymb
                
                case 60:
                    self.pointer = self.tree[self.pointer].left
                    self.current = self.tree[self.pointer]
                
                case 62:
                    self.pointer = self.tree[self.pointer].right
                    self.current = self.tree[self.pointer]
                
                case 63:
                    self.position = CV(self.code,i)
                
                case 64:
                    startpos = i
                    endpos = FMB(["[", "{", "@", ":"], ["]", "}", "a", "c"]. startpos)
                    br = ""
                    for j in range(startpos+1,endpos):
                        br += self.code[i]
                    if self.current.left >= len(self.tree.tree) and self.current.right >= len(self.tree.tree): Run(br)
                
                case 65:
                    self.AX = CV(self.code,i)
                
                case 66:
                    self.BX = CV(self.code,i)
                
                case 67:
                    self.pointer = self.tree.tree.index(self.tree.tree.bottomup_L(CV(self.code,i)))
                    self.current = self.tree.tree[self.pointe]
                
                case 68:
                    self.CX = CV(self.code,i)
                
                case 69:
                    self.pointer = self.tree.tree.index(self.tree.tree.bottomup_R(CV(self.code,i)))
                    self.current = self.tree.tree[self.pointer]
                
                case 70:
                    self.DX = CV(self.code,i)
                
                case 75:
                    self.code = self.code[:i] + self.code[i+1:]
                    if self.code[i+1] == "(":
                        self.code = self.code[:FMB(["("], [")"], i+1)] + self.code[FMB(["("], [")"], i+1)+1:]
                    self.brk = True
                
                case 78:
                    self.stack.stack[-1] += 1
                    self.ESP = len(self.stack.stack) - 1
                
                case 81:
                    print(chr(self.stack.pop()))
                
                case 83:
                    self.ESI = CV(self.code,i)
                
                case 86:
                    self.tree.insert(CV(self.code,i))
                
                case 87:
                    self.pointer = self.tree.tree.index(self.tree.tree.search(CV(self.code,i)))
                    self.current = self.tree.tree[self.pointer]
                
                case 88:
                    self.EDI = CV(self.code,i)
                
                case 91:
                    startpos = i
                    endpos = FMB(["[", "{", "@", ":"], ["]", "}", "a", "c"], startpos)
                    br = ""
                    for j in range(startpos+1,endpos):
                        br += self.code[j]
                    if self.current.left < len(self.tree.tree) and self.current.right < len(self.tree.tree): Run(br)
                
                case 94:
                    self.pointer = self.current.parent
                    self.current = self.tree.tree[self.pointer]
                
                case 100:
                    self.pointer = self.tree.tree.index(searchleaf_bottomup(CV(self.code,i),self.current,self.tree.tree))
                    self.current = self.tree.tree[self.pointer]
                
                case 115:
                    self.code = self.code[:i] + self.code[i+1:]
                    self.brk = True
                
                case 118:
                    self.tree.tree = insertleaf(CV(self.code,i),self.current,self.tree.tree)
                
                case 119:
                    self.pointer = self.tree.tree.index(searchleaf(CV(self.code,i),self.current,self.tree.tree))
                    self.current = self.tree.tree[self.pointer]
                
                case 123:
                    startpos = i
                    endpos = FMB(["[","{","@",":"],["]","}","a","c"],startpos)
                    br = ""
                    for j in range(startpos+1,endpos):
                        br += self.code[j]
                    if self.current.key != 0: Run(br)
                
                case 126:
                    if not IsIn(chr(prevop),self.registers): self.stack.pop()
                    else:
                        match prevop:
                            case 65: self.AX = self.stack.pop()
                            case 66: self.BX = self.stack.pop()
                            case 68: self.CX = self.stack.pop()
                            case 70: self.DX = self.stack.pop()
                            case 83: self.ESI = self.stack.pop()
                            case 88: self.EDI = self.stack.pop()
        
        def BlockExec(code, i):
            if i + 1 < len(code):
                self.EIP = i+1
                self.IP = i
            symb = code[i]
            if self.SNI == False and self.quote == False: FOC(ord(code[i-1]),ord(symb),i)
            else: SNI = False
