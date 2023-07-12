
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* node constructor given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, HSLAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),LT(NULL),RB(NULL)
	{}

/* destructor given */
twoDtree::~twoDtree(){
	clear();
}

/* copy constructor given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}


/* operator= given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

/* twoDtree constructor */
twoDtree::twoDtree(PNG & imIn){ 
	/* your code here */
	width = imIn.width();
	height = imIn.height();
	
	stats s(imIn);
	root = buildTree(s, pair<int, int>(0,0), pair<int, int>(width-1, height-1), true);
}

/* buildTree helper for twoDtree constructor */
twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	/* your code here */
	int x1 = ul.first;
	int y1 = ul.second;
	int x2 = lr.first;
	int y2 = lr.second;
	twoDtree::Node * curr = new Node(ul, lr, s.getAvg(ul, lr));

	// reaching leaf
	if (x1 == x2 && y1 == y2) {
		return curr;
	}
	// 1 x h, cut horizontally
	else if (x1 == x2 && y2 > y1) {
		int splitY = split(s, ul, lr, false);
		curr->LT = buildTree(s, ul, pair<int, int>(x2, splitY), false);
		curr->RB = buildTree(s, pair<int, int>(x1, splitY+1), lr, false);
	}
	// w x 1, cut vertically
	else if (x2 > x1 && y1 == y2) {	
		int splitX = split(s, ul, lr, true);
		curr->LT = buildTree(s, ul, pair<int, int>(splitX, y2), true);
		curr->RB = buildTree(s, pair<int, int>(splitX+1, y1), lr, true);
	}
	else {
		if (vert) {
			int splitX = split(s, ul, lr, vert);;
			curr->LT = buildTree(s, ul, pair<int, int>(splitX, y2), !vert);
			curr->RB = buildTree(s, pair<int, int>(splitX+1, y1), lr, !vert);
		} else {
			int splitY = split(s, ul, lr, vert);;
			curr->LT = buildTree(s, ul, pair<int, int>(x2, splitY), !vert);
			curr->RB = buildTree(s, pair<int, int>(x1, splitY+1), lr, !vert);
		}
				
	}
	return curr;
}

/* render your twoDtree into a png */
PNG twoDtree::render(){
	/* your code here */
	PNG pic(width, height);
	renderTree(root, pic);
	return pic;
}


void twoDtree::renderTree(Node* root, PNG &pic) {
	if (root == NULL) {
		return;
	}
	if (root->LT == NULL && root->RB == NULL) {
		int x1 = root->upLeft.first;
		int y1 = root->upLeft.second;
		int x2 = root->lowRight.first;
		int y2 = root->lowRight.second;
		// if normal pic, then a leaf is just one pixel;
		// if pruned pic, then a leaf contains several pixels
        for (int i = x1; i <= x2; i++){
            for(int j = y1; j <= y2; j++){
				*(pic.getPixel(i,j)) = root->avg;
            }
        }
	} else {
		renderTree(root->LT, pic);
		renderTree(root->RB, pic);
	}
}

/* prune function modifies tree by cutting off
 * subtrees whose leaves are all within tol of 
 * the average pixel value contained in the root
 * of the subtree
 */
void twoDtree::prune(double tol){
	/* your code here */
	prune(root, tol);
}


void twoDtree::prune(Node* &curr, double tol) {
	if (curr == NULL) {
		return;
	}
	HSLAPixel ref = curr->avg;
    if (shouldPrune(curr, ref, tol)) {
		// trim node's children but keep node
		clearTree(curr->LT);
        clearTree(curr->RB);
    } else {
		prune(curr->LT, tol);
        prune(curr->RB, tol);
	}
}

bool twoDtree::shouldPrune(Node* &curr, HSLAPixel &ref, double tol){
    if (curr->LT == NULL && curr->RB == NULL){
		return ref.dist(curr->avg) <= tol; 
    }
	else {
		return shouldPrune(curr->LT, ref, tol) && shouldPrune(curr->RB, ref, tol);
	}
}

/* helper function for destructor and op= */
/* frees dynamic memory associated w the twoDtree */
void twoDtree::clear() {
	/* your code here */
	if (root == NULL) {
		return;
	}
	clearTree(root);
}


void twoDtree::clearTree(Node* &curr) {
	// postorder delete
	if (curr != NULL) {
        clearTree(curr->LT);
        clearTree(curr->RB);
        delete curr;
		curr = NULL;
    }
	// // preorder delete (discarded)
	// if (curr != NULL) {
	// 	Node* left = curr->LT;
	// 	Node* right = curr->RB;
	// 	delete curr;
	// 	curr = NULL;
	// 	clearNode(left);
	// 	clearNode(right);
	// }
}


/* helper function for copy constructor and op= */
void twoDtree::copy(const twoDtree & orig){
	/* your code here */
	width = orig.width;
	height = orig.height;
	root = new Node(orig.root->upLeft, orig.root->lowRight, orig.root->avg);
	copyTree(root, orig.root);		
}


void twoDtree::copyTree(Node* &curr, Node* other) {
	if (other->LT == NULL && other->RB == NULL) {
		return;
	}
	else if (other->LT != NULL && other->RB == NULL) {
		curr->LT = new Node(other->LT->upLeft, other->LT->lowRight, other->LT->avg);
		copyTree(curr->LT, other->LT);	
	}
	else if (other->LT == NULL && other->RB != NULL) {
		curr->RB = new Node(other->RB->upLeft, other->RB->lowRight, other->RB->avg);
		copyTree(curr->RB, other->RB);
	}
	else {
		curr->LT = new Node(other->LT->upLeft, other->LT->lowRight, other->LT->avg);
		curr->RB = new Node(other->RB->upLeft, other->RB->lowRight, other->RB->avg);
		copyTree(curr->LT, other->LT);
		copyTree(curr->RB, other->RB);
	}
}


int twoDtree::split(stats & s, pair<int,int> ul, pair<int,int> lr, bool vert) {
	int x1 = ul.first;
	int y1 = ul.second;
	int x2 = lr.first;
	int y2 = lr.second;
	double w = (double) s.rectArea(ul, lr);
	double minEntropy = s.entropy(ul, lr);
	int minIdx;
	if (vert) {
		minIdx = x1;
		for (int i = x1; i < x2; i++) {
			double w1 = (double) s.rectArea(ul, pair<int,int>(i,y2));
			double w2 = w - w1;
			double entropy1 = s.entropy(ul, pair<int,int>(i,y2));
			double entropy2 = s.entropy(pair<int,int>(i+1,y1), lr);
			double eAvg = (w1 * entropy1 + w2 * entropy2) / w;
			if (eAvg <= minEntropy) {
				// in case of tie..
				if (eAvg == minEntropy) {
					int minRightArea = (y2-y1+1) * (x2-(minIdx+1)+1);
					int currRightArea = (y2-y1+1) * (x2-(i+1)+1);
					if (currRightArea < minRightArea) {
						minEntropy = eAvg;
				        minIdx = i;
					}
				}
				else {
					minEntropy = eAvg;
				    minIdx = i;
				}	
			}
		}
	} else {
		minIdx = y1;
		for (int i = y1; i < y2; i++) {
			double w1 = (double) s.rectArea(ul, pair<int,int>(x2,i));
			double w2 = w - w1;
			double entropy1 = s.entropy(ul, pair<int,int>(x2,i));
			double entropy2 = s.entropy(pair<int,int>(x1,i+1), lr);
			double eAvg = (w1 * entropy1 + w2 * entropy2) / w;
			if (eAvg <= minEntropy) {
				// in case of tie..
				if (eAvg == minEntropy) {
					int minDownArea = (x2-x1+1) * (y2-(minIdx+1)+1);
					int currDownArea = (x2-x1+1) * (y2-(i+1)+1);
					if (currDownArea < minDownArea) {
						minEntropy = eAvg;
				        minIdx = i;
					}
				}
				else {
					minEntropy = eAvg;
				    minIdx = i;
				}
			}			
		}
	}
	return minIdx;
}



