#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include<Windows.h>
#include"psapi.h"
//Node Structure
typedef struct node {
    unsigned char ch;       //The character contained by the node
    unsigned int value;    //The node's Weight
    int rank;               //The node's order
    struct node *left;      //Pointer to the left son
    struct node *right;     //Pointer to the right son
    struct node *father;    //Pointer to the father
}Node;


//Global variables
FILE *compressedfile;        
FILE *fileInput;            
FILE *decompressedfile;      
Node *array[257];           
Node *zero;					
Node *root;			


void create_tree() {
    root = malloc(sizeof(Node));  
    root->rank = 1000;  
    root->value = 0; 
    root->left=NULL;
    root->right=NULL;   
    root->father=NULL;

    zero = malloc(sizeof(Node));
    zero->rank = 998;
    zero -> value = 0;
    zero->left=NULL;
    zero->right=NULL;
    zero->father=NULL;
}

void chain_left(Node* p1, Node*p2){
    p1->left = p2;      
    p2->father =p1;     
}

void chain_right(Node* p1, Node* p2){
    p1->right = p2;     
    p2->father = p1;    
}

int right_child(Node* pn){
    if(pn->father==NULL){
        return 0;
    }

    if(pn->father->left==pn){
        return 0;
    } else {
        return 1;
    }
}

int left_child(Node* pn){
    if(pn->father==NULL){
        return 0;
    } else {
        if(right_child(pn)==true){
            return 0;
        } else {
            return 1;
        }
    }
}

Node* New_Node(Node* root, unsigned char ch){
    Node* tempZero = array[256];                    //Node zero recovery.

    Node* new = (Node*)malloc(sizeof(Node));      //Creation of a new node (Parent of the node whose character has just appeared and of the zero node)
    Node* leaf = (Node*)malloc(sizeof(Node));     //Creation of the node whose character has just appeared.

    new->ch = -1;                                 //We set the non-leaf node character with an invalid value.
    new->rank = tempZero->rank;                   //The order of the non-leaf node will be the same as that of node zero so far.
    new->value = 0;                               //The weight of the new node is set to zero, it will be updated later.
    new->left=NULL;
    new->right=NULL;

    leaf->ch = ch;                                //The new node character is the new character just found.
    leaf->rank = tempZero->rank-1;                //The order of the leaf node is one less than that of the zero node so far.
    leaf->value= 0;                               //The weight of the new node is set to zero, it will be updated later.
    leaf->left=NULL;
    leaf->right=NULL;
    tempZero->father->left =  new;                //The old parent of node zero becomes the parent of the new non-leaf node.
    new->father = tempZero->father;
    tempZero->rank -= 2;                            //Decrease the order of node zero by 2. This procedure ensures that orders are kept.
    chain_left(new, tempZero);               //Connecting the non-leaf node to node zero
    chain_right(new, leaf);                   //Connecting the non-leaf node to the new node.
    return leaf;
}

void Swap_Nodes(Node* node1, Node* node2) {
    if(right_child(node1)) {                       //The second node 'takes the place' of the first as regards paternity.
        node1->father->right = node2;
    } else {
        node1->father->left = node2;
    }

    if(right_child(node2)) {                       //The first node 'takes the place' of the second as regards paternity.
        node2->father->right = node1;
    } else {
        node2->father->left = node1;
    }

    int tempRank = node1->rank;                     //Orders prior to exchange are kept in their original position.
    node1->rank = node2->rank;
    node2->rank = tempRank;

    Node* temp = node1->father;                     //Exchange of the fathers of the two nodes.
    node1->father = node2->father;
    node2->father = temp;
}

Node* Find_node_to_swap(Node *current, Node *root) {
    Node *temp = current;
    if(root->value > temp->value && root->left != NULL && root->right != NULL) 
    {
        Node* greater_left = Find_node_to_swap(temp, root->left);
        if (greater_left != NULL) 
        {
            temp = greater_left;
        }
        Node* greater_right = Find_node_to_swap(temp, root->right);
        if (greater_right != NULL) 
        {
            temp = greater_right;
        }
    } 
    else if (root->value == temp->value && root->rank > temp->rank) 
    {
        temp = root;
    } 
    if (temp != current){
        return temp;
    } else {
        return NULL;
    }
}

void Elaborate_Tree(Node* current, Node* root) {

    while(current != root) {
        
        //printf("%u ",current->value);
        Node* node_to_exchange = Find_node_to_swap(current, root); //See if there is a node to trade with
        if(node_to_exchange!=NULL && node_to_exchange->right != current) { //If a node to swap is found and this is not the parent (case where the children are node zero + current node)
            //printf("debug1\n");
            Swap_Nodes(current, node_to_exchange); //The exchange is made between the two nodes
            //printf("debug2\n");
        }
        else{
            //printf("no swap \n");
        }
        (current->value)++; //Weight increase of the current node
        current = current->father; // go up one level by pointing to the parent of the current node
        
    }
    (root->value)++;
}

int Read_Bit(FILE* file, int Num_Final_Bits, unsigned long int File_Size){
    static char ch; // char where the byte being read is saved
    static int counter = -1; //counter indicating how many bits are still to be read from 'ch'.
    int r_bit;
    static int file_counter=0;
    if(counter == -1){ //If counter == -1 it means that the byte saved in ch has been read in its entirety, so a new reading must be performed.
        if(fread(&ch,sizeof(char),1,file)){
            counter = 7;
            file_counter++;
        } else {
            return -1; //if fread is not executed successfully, it means that you have reached the end of the file.
        }
    }
    /*
    When the penultimate byte of the compressed file is reached in reading the file (File_Size == ftell (file))
     and the number of bits written (7-counter) is greater than or equal to the number of "terminal" bits, the reading is stopped.
     Num_Final_Bits equals the number of relevant bits written at the end of the compression ("terminal" bits).
     Alternatively, you can check with ftell that the cursor does not go beyond the penultimate byte, in this case -1 is returned
     */

    if((File_Size == file_counter && Num_Final_Bits <= (7 - counter)) || (file_counter > File_Size)) {
      return -1;
    }
    r_bit = (ch >> counter) & 0x01; // r_bit contains the value of the bit to be returned (0 or 1) which is obtained by shifting ch of the counter value to the right and performs a mask with 0x01

    counter--;
    return r_bit;
}

int* Read_Char(char ch){
  int* c = malloc(sizeof(int)*8);
    for(int i = 7; i >= 0; i--) {
        c[7-i] = (ch >> (i)) & 0x01; // with the for loop you read one bit at a time and 'place' it inside an int.
    }
    return c;
}

int findDepth(Node* pn){
    int counter=0;
    Node* temp = pn;

    while(temp->father!=NULL){              //Loop that increments a counter every time the node has a parent.
        temp = temp->father;                //The counter increases for each father.
        counter++;
    }
    return counter;                         //The counter is the depth of the node.
}

int* findPath(Node* n, int depth){                  // We pass as argument the node of which to find the path and the depth at which it is found, found with the findPath method
    int* path = malloc(sizeof(int)*depth);          //We allocate the space needed to contain the path of the file, which will have as many ints as its depth.
    int i = 0;

    Node* temp = n;
    while(temp->father!=NULL){                      //Starting from the node, we go up the tree up to the root (father == NULL), and we create the path code in reverse.
        if(right_child(temp)==1){                  //We use the right_child / left methods to check if a node is a left or right child of the father.
            path[i]=1;
        } else if(left_child(temp)==1){
            path[i]=0;
        }
        if(temp->father!=NULL){
            temp = temp->father;
        }
        i++;
    }
    for(int z = 0; z<depth/2; z++){                 //Once we reach the root, we turn the path, so as to have it starting from the root to the leaf and not the other way around.
        int t = path[z];
        path[z] = path[depth-1-z];
        path[depth-1-z] = t;
    }

    return path; //we return the correct path from the root to the node passed as an argument
}

void Create_output_Code(int path[], int dimension, int Final){
    static unsigned char written; //static char where the bits to be written are saved
    static int n = 8; //int value indicating how much to shift the bit, to be written, to the left.
    static unsigned char bit; //bit to insert in written
    int counter1=0;
    if(Final) 
    { //flag that indicates if the last writing has been reached, so if the writing must be done even if the char written is not complete
        for (int i = 0; i < dimension; i++) 
        {

            n--;
            bit = ((unsigned char) path[i]) << (n); // it shifts left by a value of n to "queue" the bits. If you have 0 bits written, n = 7, then shift the character in path [i] by 7 on the left.
            written = written | bit;
            /* With an 'OR' I update the written char by adding the new bit.
             When n == 0 then we will have written 'full' and write to file, putting written back to 0 and reporting n = 8.
            */
            if (n == 0) 
            {
                n = 8;
                fwrite(&written, sizeof(unsigned char), 1, compressedfile);
                written = 0x00;
            }
        }
    } else 
    {
      // The last writing is made. A byte is written containing the last relevant 'left' bits and then a further byte whose value will be equal to the number of relevant bits written in the previous byte.
        int quantity = 8-n;
        fwrite(&written, sizeof(unsigned char), 1, compressedfile);
        
        fwrite(&quantity, sizeof(unsigned char), 1, compressedfile);
    }
    // At the end of the call the memory occupied by the array path is freed
    free(path);
}

void Create_Char_Output_Code(int* path, int dimension, char* ch){
    Create_output_Code(path, dimension, -1);
    int* c;
    c = Read_Char(*ch);
    Create_output_Code(c, 8, -1);
}

void Deallocate_Tree(Node* current) {
  if (current == NULL) {
    return;
    
  }
    Deallocate_Tree(current->left);
    Deallocate_Tree(current->right);
    free(current);

  }

int main(int argc, char**argv){
    PROCESS_MEMORY_COUNTERS_EX pmc;
    clock_t t;
    t= clock();
    if(argc!=4){
        printf("The program requires 3 arguments: \n '-c' coding or '-d' decoding \n input file name \n output file name.");
    } else {
        if((fileInput = fopen(argv[2],"rb") )!= NULL){  //We check if the file passed as argument exists

            if(strcmp(argv[1],"-c")==0){ // If file exists we check whether to proceed with compression or decompression
                printf("Coding...\n");
                compressedfile = fopen(argv[3] ,"wb"); //The compressed file is created

                /* 
                To the root node we attribute as rank the arbitrary value 1000. The node 0, being left child, will have rank 998
                */

                create_tree();
                array[256] = zero;

                unsigned char current; //char in which we save the byte read

                while(fread(&current,sizeof(unsigned char),1,fileInput)){
                   
                    if(array[current]!=NULL){
                        //If there is already a node that contains the character read, we write the path of the node and then we adapt the tree.

                        Node* temp = array[current];                  //Node identification

                        int tempDepth = findDepth(temp);                  //Identification of the depth of this node
                        int* thepath = findPath(temp, tempDepth);
                        Create_output_Code(thepath,tempDepth, -1);  //Writing thepath to the file

                        //Adaptation of the tree
                        //printf("%u ",current);
                        Elaborate_Tree(temp, root);
                    } 
                    else 
                    {
                        // If the character is new, we write the thepath for node zero and then the character.
                        int tempDepth = findDepth(array[256]);  //Let's find the depth of node zero.
                        if(tempDepth==0){ //If it is the first character encountered, we create the node, we connect it to the right of the root, to which we also connect node 0
                        /*    int counter1=7;
                            int r_bit;
                            for(int w=0;w<8;w++)
                            {
                                r_bit = (current >> counter1) & 0x01; // r_bit contains the value of the bit to be returned (0 or 1) which is obtained by shifting ch of the counter value to the right and performs a mask with 0x01
                                printf("%d ",r_bit);
                                counter1--;
                            }
                        */
                            fwrite(&current,sizeof(char),1,compressedfile);

                            Node* first = (Node*)malloc(sizeof(Node));
                            first->ch = current;
                            first->rank = 999;
                            first->value=0;
                            first->left =NULL;
                            first->right=NULL;
                            chain_right(root, first);
                            chain_left(root,zero);
                            //printf("%u ",first->ch);
                            Elaborate_Tree(first,root);
                            array[current] = first;
                        } else {
                            /*
                             * If it is not the first character (tree with at least 3 nodes: root, zero and character) we find the thepath of node 0
                              * We save on compressedfile the thepath of node zero and of the character to be added, then create a new node containing the character saved in current.
                              * Let's proceed with the elaboration of the tree starting from the new node
                             */
                            int* thepath = findPath(zero, tempDepth);
                            unsigned char* ptCurrent = &current;
                            Create_Char_Output_Code(thepath,tempDepth,ptCurrent);
                            Node* temp = New_Node(root,current);
                            array[current] = temp;
                            //printf("%u",current);
                            Elaborate_Tree(temp, root);
                        }

                    }
                }
                Create_output_Code(0, 0, 0);
                Deallocate_Tree(root);
                printf("\nFile coded successfully\n");
            } else if(strcmp(argv[1],"-d")==0){
                printf("Decoding...\n");
                fseek(fileInput, -1, SEEK_END);
                unsigned long int File_Size = ftell(fileInput);                 //Knowing the size of the file is essential in order to be able to write the 'leftover' bits from compression.
                int bit_number_at_end=0;    
                unsigned char current;                                      //The last byte has a decimal value ranging from 0 to 8. It indicates how many bits of the penultimate byte must be considered.
                for(int w = 0; w<8; w++)
                {
                    int read=(Read_Bit(fileInput, 8, File_Size+1));
                    
                    bit_number_at_end = bit_number_at_end | (read << (7-w));  // Read the next 8 bits
                }
                //bit_number_at_end=(int)current;
                
                rewind(fileInput);
                decompressedfile = fopen(argv[3],"wb");

                //Tree initialization with root, node zero, and node containing the first character read.
                create_tree();
                array[256] = zero;

                
                fread(&current,sizeof(unsigned char),1,fileInput);
                fwrite(&current,sizeof(unsigned char),1,decompressedfile);
                Node* first = (Node*)malloc(sizeof(Node));
                first->ch = current;
                first->rank = 999;
                first->left=NULL;
                first->right=NULL;
                root->rank = 1000;
                chain_right(root,first);
                chain_left(root,zero);
                first->value = 1;
                first->father->value = 1;
                array[current] = first;

                int bitRead = 0;                   //Variable that will contain the last bit read (thanks to the Read_Bit () function)

                while(bitRead>=0){                 //Reading continues until the end of the file (which will never actually happen as we know where to stop)
                    Node* tmp = root;

                    //Descend in the tree until you find the node containing the next character.
                    while(tmp->left!=NULL && tmp->right!=NULL)
                    {
                        bitRead = Read_Bit(fileInput, bit_number_at_end, File_Size);  //Reading the bit
                        if(bitRead==1){
                            tmp = tmp->right;           //Shift to the right of the tree if the bit read is a one
                        } else if (bitRead==0){
                            tmp = tmp->left;            //Left shift of the tree if the bit read is a zero
                        } else {
                            break;                      //Case where the file has come to an end (impossible)
                        }
                    }

                    //We check if the thepath provided leads to node zero or to a known node, and we act accordingly.
                    if(bitRead>=0){                    //In case the file has not reached the end.
                        if(tmp==array[256]){            //If the node found is node zero, we proceed to write the next character to file and add it to the tree
                            current = 0x00;
                            for(int w = 0; w<8; w++){
                                int read=(Read_Bit(fileInput, bit_number_at_end, File_Size));
                                current = current | (read << (7-w));  // Read the next 8 bits
                            }

                            Node* new = New_Node(root,current);                        //Creation of the new node containing the character.
                            array[current] = new;
                            //printf("%u",current);
                            
                            fwrite(&current,sizeof(unsigned char),1,decompressedfile);
                            //printf("%u",current);
                            /*if(File_Size == (ftell(fileInput)))
                            {
                                printf("%c",current);
                            }
                            */
                            Elaborate_Tree(new,root);                                    //Tree processing, which may change due to the addition of the new node.
                        } else {
                            //printf("%u",tmp->ch);
                            fwrite(&(tmp->ch),sizeof(unsigned char),1,decompressedfile);     //If the node found is not node zero, it means that we can write the character of that node.
                            Elaborate_Tree(tmp,root);
                        }
                    }
                    if(bitRead == -1 && tmp!=array[256])
                    {
                        //printf("%c",tmp->ch);
                    }
                }

                Deallocate_Tree(root);
                printf("\nDecoding successful.\n");
            } else {
                printf("Invalid option, please select '-c' if coding or '-d' if decoding\n");
            }
        } else {
            printf("File not found.\n");
        }
    }
    t = clock() -t;
    double timetaken = ((double)t)/CLOCKS_PER_SEC;
    fprintf(stderr, "Seconds of execution %f\n", timetaken);
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
    printf("virtual memory used:%lu bytes\n RAM used:%lu bytes",virtualMemUsedByMe,physMemUsedByMe);
}





