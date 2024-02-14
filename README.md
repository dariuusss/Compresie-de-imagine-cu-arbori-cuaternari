# Compresie-de-imagine-cu-arbori-cuaternari


TNod is the data type utilized for implementing the quaternary tree.

TMatrix is the data type used for the pixel matrix.

List is the data type used for the information list for nodes as per requirement 3.

The AllocateNode function allocates and initializes a node in the quaternary tree.

The allocate_list_element function allocates and initializes an element from the information list used within requirement 3.

The divide_matrix function takes parameters such as the pixel matrix, the node to which it could potentially attach some children, an indicator for the corresponding child of a possible attachment, the compression factor, the address of the variable where the maximum size of an undivided square will be stored, and the output file. This function constructs the compression tree and calculates the maximum side of an undivided square and the corresponding color scores, presenting two scenarios: the current node is terminal or internal. If it is terminal, no recursive calls are made to divide the pixel matrix. If the node is internal, then 2 midpoint indices for rows and columns are calculated in the current area of the matrix, and the function is recursively called 4 times, each call corresponding to one quarter of the current area of the pixel matrix.

The number_of_levels function calculates the index of the last level in the compression tree.

The number_of_terminal_nodes function counts the number of terminal nodes in the compression tree.

The fill_matrix function takes parameters such as the size of the pixel matrix, the pixel matrix itself, the indices corresponding to the first row, last row, first column, and last column of the current area of the pixel matrix, and the node from the compression tree corresponding to that area of the matrix, effectively completing the pixel matrix for the image. If the node for which the function is called is terminal, then all pixels in the current area of the matrix will have the same color channel values as the previously mentioned node. If it is internal, the current area is recursively divided into 4 quarters.

The delete_quaternary_tree function frees the memory allocated for the quaternary tree. To avoid losing connections to children, we first delete all 4 sub-trees of the node for which the function is called, and finally, we delete that node.

For solving requirement 1, we read the pixel matrix, construct the compression tree, and determine the requested statistics.

For solving requirement 2, we read the pixel matrix, construct the compression tree, and determine its breadth-first traversal. For this, we construct a levels matrix where index i contains all nodes at level i (the root is at level 0). We start from index 0, where we insert the root. On index line 1, we will find the children of the root. On index line 2, we will find all children of nodes at level 1, and so on. In other words, level k contains all children of nodes at level k-1, and the total number of nodes at level k is 4 times the number of non-terminal nodes at level k-1. Finally, we traverse the levels matrix from left to right and top to bottom, writing the node data to the requested binary file.

For solving requirement 3, we read the data from the file and save the values corresponding to the nodes in a list. If the current element is 0, then we have an internal node, we construct it, and advance in the list by one position. If the current value is 1, then we have a terminal node, we construct it, and advance in the list by 4 positions, because the next 3 are the color values for that node. We repeat the process until we finish traversing the list. After determining how many nodes we have, we repeat the previous procedure, but this time, we also build a vector of nodes, which will contain the nodes of the compression tree, but without established connections. Once we have the nodes of the compression tree, we build it in a similar manner to obtaining the breadth-first traversal from requirement 2: starting from the root, putting its children at the next level, and repeating the process until all nodes are inserted into the tree. The only difference is that this time we directly obtain the tree instead of a representation of it through a matrix that retains its breadth-first traversal. Finally, we allocate space for the pixel matrix and call the fill_matrix function, which will initialize the pixels with the corresponding values from the nodes of the compression tree.








