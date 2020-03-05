#include "queue.h"



QNode *newQNode(int d, unsigned long long p) {
   QNode *temp = (QNode *) malloc(sizeof(QNode));
   if(!temp) {return NULL;}
   temp->data = d;
   temp->priority = p;
   temp->next = NULL;
   return temp;
}

void push(QNode** head, int d, unsigned long long p) {
   if(*head==NULL) {*head=newQNode(d,p);return;}

   QNode *start = (*head);
   // Create new QNode
   QNode *temp = newQNode(d, p);
   if(!temp) {
       while(!isEmpty(head))
           pop(head);
       return;
   }

   // Special Case: The head of list has lesser
   // priority than new node. So insert new
   // node before head node and change head node.
   if ((*head)->priority > p) {

	  // Insert New QNode before head
	  temp->next = *head;
	  (*head) = temp;
   } else {
	  // Traverse the list and find a
	  // position to insert new node
	  while (start->next != NULL &&
			 start->next->priority < p) {
		 start = start->next;
	  }

	  // Either at the ends of the list
	  // or at required position
	  temp->next = start->next;
	  start->next = temp;
   }
}

int pop(QNode **head) {
   int data=peek(head);
   QNode *temp = *head;
   (*head) = (*head)->next;
   free(temp);
   return data;
}

int peek(QNode **head) {
   return (*head)->data;
}

int isEmpty(QNode **head) {
   return (*head) == NULL;
}
void decrease(QNode** head, int d, unsigned long long p) {
    if(*head==NULL) return;
    QNode* prev = (*head);
    if(prev->data==d && p<prev->priority){
        *head = prev->next;
        free(prev);
        push(head,d,p);
        return;
    }
    while(prev->next) {
        if(prev->next->data==d &&  p<prev->priority) {
            QNode* garbage=prev->next;
            prev->next=prev->next->next;
            free(garbage);
            push(head,d,p);
            return;
        }
        prev=prev->next;
    }


}
