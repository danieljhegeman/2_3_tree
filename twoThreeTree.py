import Tree

class twoThreeTree():
  def __init__(self, vals=[]):
    self.tree = Tree.twoThreeTree()
    for val in vals:
      self.insert(val)

  def insert(self, val):
    return Tree.addNode(self.tree, val)

  def remove(self, val):
    result = Tree.removeNode(self.tree, val)
    return result
  
  def contains(self, val):
    if Tree.hasVal(self.tree, val):
      return True
    else:
      return False

  def count(self):
    return Tree.count(self.tree)
