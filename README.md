# 2-3 Tree

Python interface for a [2-3 tree](https://en.wikipedia.org/wiki/2%E2%80%933_tree) integer store, implemented in C.

## Requirements

- Python >= 3.0
- GCC

## Configuration

### Generate shared object file

Build .so file:
```
python3 setup.py build
```

Locate the .so file that is generated, and move it up to the working directory:
```
mv build/lib.linux-x86_64-3.5/Tree.cpython-35m-x86_64-linux-gnu.so Tree.cpython-35m-x86_64-linux-gnu.so
```

### Usage

Instantiate twoThreeTree object with optional array of initial integer values:
```
from twoThreeTree import twoThreeTree

myTree = twoThreeTree([1,2,3])
```

Check for presence of integers:
```
myTree.contains(1) # True
myTree.contains(0) # False
```

Insert additional single values:
```
myTree.insert(0) # True
myTree.contains(0) # True
```

Remove values:
```
myTree.remove(1) # True
myTree.contains(1) # False
```

Get number of leaves in tree:
```
myTree.count() # 3
```

### See also:

 - [2-3 Trees](http://pages.cs.wisc.edu/~vernon/cs367/notes/10.23TREE.html)
