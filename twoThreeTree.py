import Tree

class twoThreeTree():
  def __init__(self, vals=[]):
    self.__treePtr = Tree.twoThreeTree()
    for val in vals:
      self.insert(val)

  def insert(self, val):
    return Tree.addNode(self.__treePtr, val) == 1

  def remove(self, val):
    return Tree.removeNode(self.__treePtr, val) == 1
  
  def contains(self, val):
    return Tree.hasVal(self.__treePtr, val) == 1

  def count(self):
    return Tree.count(self.__treePtr)
