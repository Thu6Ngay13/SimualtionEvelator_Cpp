#include <iostream> 
#include <ctime>
#include <iomanip>
using namespace std;
const int FLOORS = 12;
const int STATES = 3;
const int UP = 0;
const int DOWN = 1;
const int STOP = 2;

struct USER{
	int in;
	int out;
	
	USER *next;
	USER *prev;
	
	void make_User(int in, int out){
		this->in = in;
		this->out = out;
		
		next = nullptr;
		prev = nullptr;
	}
	
};

struct EVELATOR{
	int floorcur;
	int state;
	
	USER *FRONT;
	USER *REAR;
	
	bool is_Empty(){ 
		return FRONT == nullptr;
	}
	
	void make_Elevator(int floorcur, int state){
		this->floorcur = floorcur;
		this->state = state;
		FRONT = nullptr;
		REAR = nullptr;
	}
	
	bool add_Que(USER *x){
		if(this->is_Empty()){
			FRONT = x;
			REAR = x;
			return true;
		}
		REAR->next = x;
		x->next = nullptr;
		x->prev = REAR;
		REAR = x;
		return true;
	}
	
	USER *delete_Que(){
		if(this->is_Empty()) return nullptr;
		
		USER *result = new USER();
		result->in = FRONT->in;
		result->out = FRONT->out;
		result->next = nullptr;
		result->prev = nullptr;
		
		if(FRONT == REAR){
			delete FRONT;
			FRONT = nullptr;
			REAR = nullptr;	
			return result;
		}
		
		USER *temp = FRONT->next;
		temp->prev = nullptr;
		
		delete FRONT;
		FRONT = temp;
		
		return result;
	}
	
	bool add_User(USER *x){
		if(this->is_Empty()){
			FRONT = x;
			REAR = x;
			return true;
		}
		
		USER *temp = FRONT;
		if(state == UP){
			while(temp != nullptr && temp->out <= x->out) 
				temp = temp->next;
		}
		else if(state = DOWN){
			while(temp != nullptr && temp->out >= x->out) 
				temp = temp->next;
		}
		
		if(temp == nullptr){
			REAR->next = x;
			x->prev = REAR;
			x->next = nullptr;
			REAR = x;
			return true;
		}
		
		if(temp->prev == nullptr){
			x->prev = nullptr;
			x->next = temp;
			FRONT->prev = x;
			FRONT = x;
			return true;
		}
		
		temp->prev->next = x;
		x->prev = temp->prev;
		
		x->next = temp;
		temp->prev = x;
	}
	
	USER *delete_User(USER *x){
		if(this->is_Empty()) return nullptr;
		
		USER *result = new USER();
		result->in = x->in;
		result->out = x->out;
		result->next = nullptr;
		result->prev = nullptr;

		if(FRONT == REAR){
			delete x;
			FRONT = nullptr;
			REAR = nullptr;	
			return result;
		}
		
		if(x->prev == nullptr){
			USER *temp = x->next;
			delete FRONT;
			temp->prev = nullptr;
			FRONT = temp;
			return result;
		}
		
		if(x->next == nullptr){
			USER *temp = x->prev;
			delete REAR;
			temp->next = nullptr;
			REAR = temp;
			return result;
		}
		
		x->prev->next = x->next;
		x->next->prev = x->prev;
		delete x;
		x = nullptr;
		
		return result;
	}
};

void exe_UP(EVELATOR &eve, EVELATOR &que){
	if(eve.is_Empty() && que.is_Empty()) return;
	if(eve.state != UP) return;
	
	int outMax = eve.floorcur;
	USER *pos = que.FRONT;
	
	while(pos != nullptr){
		USER *x = pos;
		pos = pos->next;
		if(eve.floorcur <= x->in && x->in < x->out){
			USER *temp = que.delete_User(x);
			eve.add_User(temp);
			outMax = max(outMax, temp->out);
		}
	}
	
	if(eve.is_Empty()){
		que.state = eve.state;
		eve.state = STOP;
		return;
	}

	for(int i = eve.floorcur; i <= outMax; i++){
		eve.floorcur = i;
		if(!eve.is_Empty() && i == eve.FRONT->out){
			cout << setw(30) << left << "STATE: UP -> STOP" << "\t"
				<< "FLOOR CURRENT: " << i << endl;
			que.state = eve.state;
			eve.state = STOP;
			
			while(!eve.is_Empty() && i == eve.FRONT->out){
				USER *user = eve.delete_Que();
				delete user;
				user = nullptr;
				cout << setw(30) << left << "STATE: STOP" << "\t" 
					<< "FLOOR CURRENT: " << i << "\t" << "USER GET OUT" << endl;
			}
			
			if(eve.is_Empty()) return;
			
			cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
				<< "FLOOR CURRENT: " << i << endl;	
			que.state = eve.state;
			eve.state = UP;
		}
		else
			cout << setw(30) << left << "STATE: UP" << "\t"
				<< "FLOOR CURRENT: " << i << endl;
	}
}

void exe_DOWN(EVELATOR &eve, EVELATOR &que){
	if(eve.is_Empty() && que.is_Empty()) return;
	if(eve.state != DOWN) return;
	
	int outMin = eve.floorcur;
	USER *pos = que.FRONT;
	
	while(pos != nullptr){
		USER *x = pos;
		pos = pos->next;
		if(eve.floorcur >= x->in && x->in > x->out){
			USER *temp = que.delete_User(x);
			eve.add_User(temp);
			outMin = min(outMin, temp->out);
		}
	}
	
	if(eve.is_Empty()){
		que.state = eve.state;
		eve.state = STOP;
		return;
	}

	for(int i = eve.floorcur; i >= outMin; i--){
		eve.floorcur = i;
		if(!eve.is_Empty() && i == eve.FRONT->out){
			cout << setw(30) << left << "STATE: DOWN -> STOP" << "\t" 
				<< "FLOOR CURRENT: " << i << endl;
			que.state = eve.state;
			eve.state = STOP;
			
			while(!eve.is_Empty() && i == eve.FRONT->out){
				USER *user = eve.delete_Que();
				delete user;
				user = nullptr;
				cout << setw(30) << left << "STATE: STOP" << "\t" 
					<< "FLOOR CURRENT: " << i << "\t" << "USER GET OUT" << endl;
			}
			
			if(eve.is_Empty()) return;
			
			cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t" 
			<< "FLOOR CURRENT: " << i << endl;	
			que.state = eve.state;
			eve.state = UP;
		}
		else
			cout << setw(30) << left << "STATE: DOWN" << "\t" 
				<< "FLOOR CURRENT: " << i << endl;	
	}
} 

void exe_STOP(EVELATOR &eve, EVELATOR &que){	
	if(eve.is_Empty() && que.is_Empty()) return;
	if(eve.state != STOP) return;
	
	if(que.state == STOP){
		if(que.FRONT->in < que.FRONT->out){
			if(eve.floorcur > que.FRONT->in && que.FRONT->out > eve.floorcur){
				cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				while(eve.floorcur > que.FRONT->in){
					cout << setw(30) << left << "STATE: DOWN" << "\t" 
						<< "FLOOR CURRENT: " << eve.floorcur << endl;	
					eve.floorcur--;
				}
				cout << setw(30) << left << "STATE: DOWN -> STOP" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				
			}
			else
				cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;	
			
			que.state = eve.state;
			eve.state = UP;
		}
		else{
			if(eve.floorcur < que.FRONT->in && que.FRONT->out < eve.floorcur){
				cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				while(eve.floorcur < que.FRONT->in){
					cout << setw(30) << left << "STATE: UP" << "\t" 
						<< "FLOOR CURRENT: " << eve.floorcur << endl;	
					eve.floorcur++;
				}
				cout << setw(30) << left << "STATE: UP -> STOP" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;
				
			}
			else
				cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;	
			
			que.state = eve.state;
			eve.state = DOWN;
		}
		return;		
	}
	
	int inMin = eve.floorcur;
	int inMax = eve.floorcur;
	USER *pos = que.FRONT;
	
	while(pos != nullptr){
		inMin = min(inMin, pos->in);
		inMax = max(inMax, pos->in);
		pos = pos->next;
	}
	
	if(que.state == UP){
		if(eve.floorcur < inMax){
			cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			while(eve.floorcur < inMax){
				cout << setw(30) << left << "STATE: UP" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;	
				eve.floorcur++;
			}
			cout << setw(30) << left << "STATE: UP -> STOP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t"
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
		}
		else{
			cout << setw(30) << left << "STATE: UP -> STOP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t"
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
		}
		
		que.state = eve.state;
		eve.state = DOWN;
	}
	else{
		if(eve.floorcur > inMin){
			cout << setw(30) << left << "STATE: STOP -> DOWN" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			while(eve.floorcur > inMin){
				cout << setw(30) << left << "STATE: DOWN" << "\t" 
					<< "FLOOR CURRENT: " << eve.floorcur << endl;	
				eve.floorcur--;
			}
			cout << setw(30) << left << "STATE: DOWN -> STOP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
		}
		else{
			cout << setw(30) << left << "STATE: DOWN -> STOP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
			cout << setw(30) << left << "STATE: STOP -> UP" << "\t" 
				<< "FLOOR CURRENT: " << eve.floorcur << endl;
		}
			
		que.state = eve.state;
		eve.state = UP;
	}
}

void Xuly(){
	srand(time(NULL));
	
//	EVELATOR eve; eve.make_Elevator(rand() % (FLOORS-1) + 1, rand() % STATES);
//	EVELATOR que; que.make_Elevator(-1, 2);

//	int n = rand() % 14 + 1; cout << "NUMBERS OF USER: " << n << endl;
//	for(int i = 0; i < n; i++){
//		USER *user = new USER();
//		int in = rand() % FLOORS;
//		int out = rand() % FLOORS;
//		while(in == out) out = rand() % FLOORS;
//		user->make_User(in, out);
//		que.add_Que(user);
//		cout << "USER: " << i+1 << " - " << "IN: " << in << " " << "OUT: " << out << endl;
//	}
	
	EVELATOR eve; eve.make_Elevator(1, 1);
	EVELATOR que; que.make_Elevator(-1, 2);
	
	cout << "EVELATOR STATE: " << (eve.state ? ((eve.state % 2) ? "DOWN" : "STOP") : "UP") 
		<< " " << "- FLOOR CURRENT: " << eve.floorcur << endl;

	USER *user1 = new USER();
	user1->make_User(7, 5);
	que.add_Que(user1);
	cout << "USER: " << 1 << " - " << "IN: " << user1->in << " " << "OUT: " << user1->out << endl;
	
	USER *user2 = new USER();
	user2->make_User(7, 8);
	que.add_Que(user2);
	cout << "USER: " << 2 << " - " << "IN: " << user2->in << " " << "OUT: " << user2->out << endl;
	
	USER *user3 = new USER();
	user3->make_User(4, 2);
	que.add_Que(user3);
	cout << "USER: " << 3 << " - " << "IN: " << user3->in << " " << "OUT: " << user3->out << endl;
	
	USER *user4 = new USER();
	user4->make_User(3, 7);
	que.add_Que(user4);
	cout << "USER: " << 4 << " - " << "IN: " << user4->in << " " << "OUT: " << user4->out << endl;
	
	USER *user5 = new USER();
	user5->make_User(10, 6);
	que.add_Que(user5);
	cout << "USER: " << 5 << " - " << "IN: " << user5->in << " " << "OUT: " << user5->out << endl;
	
	USER *user6 = new USER();
	user6->make_User(3, 9);
	que.add_Que(user6);
	cout << "USER: " << 6 << " - " << "IN: " << user6->in << " " << "OUT: " << user6->out << endl;

	USER *user7 = new USER();
	user7->make_User(11, 0);
	que.add_Que(user7);
	cout << "USER: " << 7 << " - " << "IN: " << user7->in << " " << "OUT: " << user7->out << endl;
	
	USER *user8 = new USER();
	user8->make_User(5, 4);
	que.add_Que(user8);
	cout << "USER: " << 8 << " - " << "IN: " << user8->in << " " << "OUT: " << user8->out << endl;
	
	USER *user9 = new USER();
	user9->make_User(6, 9);
	que.add_Que(user9);
	cout << "USER: " << 9 << " - " << "IN: " << user9->in << " " << "OUT: " << user9->out << endl;
	
	USER *user10 = new USER();
	user10->make_User(9, 2);
	que.add_Que(user10);
	cout << "USER: " << 10 << " - " << "IN: " << user10->in << " " << "OUT: " << user10->out << endl;
	
	USER *user11 = new USER();
	user11->make_User(7, 10);
	que.add_Que(user11);
	cout << "USER: " << 11 << " - " << "IN: " << user11->in << " " << "OUT: " << user11->out << endl;
	
	USER *user12 = new USER();
	user12->make_User(3, 2);
	que.add_Que(user12);
	cout << "USER: " << 12 << " - " << "IN: " << user12->in << " " << "OUT: " << user12->out << endl;
	
	while(!eve.is_Empty() || !que.is_Empty()){
		exe_UP(eve, que);
		exe_DOWN(eve, que);
		exe_STOP(eve, que);
	}	
}

int main(){
	Xuly();
	return 0;
}
