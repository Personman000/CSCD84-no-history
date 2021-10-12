#include <stdbool.h>
#include <stdio.h>

bool find_node(int x, int y, int array[][2], int length)
{
	if(length > sizeof(array))
	{
		printf("Given length is larger than array size. Array size: %d, Given Length: %d\n", sizeof(array), length);
		
		length = sizeof(array);
	}

	bool found = false;
	int i;
	for(i=0; i<length; i++)
	{
		found = found || (x == array[i][0] && y == array[i][1]);
	}
	
	return found;
}

int main()
{
    
    int a[3][2];
    a[0][0] = 1; a[0][1] = 0;
    a[1][0] = 2; a[1][1] = 3;
    a[2][0] = 4; a[2][1] = 5;
    
    //bool b = find_node(1, 3, a, 3);
    //printf("Got: %d\n", b);
    
    int* a_pointer[3][2] = &a;
    printf("a: %d, a_point: %d\n", a[0][0], a_pointer[0][0]);
    a_pointer += 1;
    printf("a: %d, a_point: %d\n", a[0][0], a_pointer[0][0]);
    
    return 0;
}
