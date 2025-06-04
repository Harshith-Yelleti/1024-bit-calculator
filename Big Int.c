#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BIGINT_SIZE 309

typedef struct BIG_INT
{
    int *num;
    int signedBit;    // 1 if negative, 0 if positive
    int length;
} BigInt;

BigInt* addBigInts(BigInt *b1, BigInt *b2);
BigInt* subtractBigInts(BigInt *b1, BigInt *b2);

void printBigInt(BigInt *b)
{
    int i;
    if(b->signedBit == 1)
        printf("-");
    for(i = b->length-1; i >= 0; i--)
        printf("%d", b->num[i]);
}

// Converts input string to BigInt data type
BigInt stringToBigInt(char *s)
{
    BigInt b;
    int size = strlen(s);
    if(s[0] == '-')
    {
        b.signedBit = 1;
    }    
    else
        b.signedBit = 0;    
    b.length = size - b.signedBit;
    b.num = (int*)malloc(sizeof(int) * (b.length));
    int *i;
    i = b.num;
    size--;
    while(size >= b.signedBit)
    {
        *(i) = s[size] - 48;
        i++;
        size--;
    }
    return b;
}

// Compares 2 BigInts
// Returns 0 if same, <0 if b2 > b1, >0 if b1 > b2
int compare(BigInt *b1, BigInt *b2)
{
    int i, retval;
    if(b1->length == b2->length)
    {
        i = (b1->length) - 1;
        while(i >= 0 && b1->num[i] == b2->num[i])
        {
            i--;
        }
        if(i < 0)
        {
            retval = 0;
        }
        else
        {
            retval = (b1->num[i]) - (b2->num[i]);
        }
    }
    else
    {
        retval = (b1->length) - (b2->length);
    }
    return retval;
}

// Addition
BigInt* addBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    int carry;
    BigInt* sum = (BigInt*)malloc(sizeof(BigInt));
    if(n2 > n1)
        sum->num = (int*)malloc(sizeof(int)*(n2 + 1));
    else
        sum->num = (int*)malloc(sizeof(int)*(n1 + 1));
    
    // If both numbers are of the same sign
    if(b1->signedBit == b2->signedBit)
    {
        sum->signedBit = b1->signedBit;
        
        carry = 0;
        int i = 0, digit;
        while(i < n1 && i < n2)
        {
            digit = (b1->num[i]) + (b2->num[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->num[i] = digit;
            i++;
        }
        while(i < n1)
        {
            digit = (b1->num[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->num[i] = digit;
            i++;
        }
        while(i < n2)
        {
            digit = (b2->num[i]) + carry;
            carry = digit / 10;
            digit = digit % 10;
            sum->num[i] = digit;
            i++;
        }
        while(carry != 0)
        {
            sum->num[i] = (carry % 10);
            carry /= 10;
            i++;
        }
        sum->length = i;
    }
    // If both numbers are of different signs
    else
    {
        if(b1->signedBit == 1)
        {
            b1->signedBit = 0;
            sum = subtractBigInts(b2, b1);
            b1->signedBit = 1;
        }
        else
        {
            b2->signedBit = 0;
            sum = subtractBigInts(b1, b2);
            b2->signedBit = 1;
        }
    }
    return sum;
}

BigInt* subtraction(BigInt *b1, BigInt *b2)
{
    BigInt* diff = (BigInt*)malloc(sizeof(BigInt));
    diff->num = (int*)malloc(sizeof(int)*(b1->length));
    int borrow = 0;
    int i = 0, digit;
    while(i < b2->length)
    {
        digit = (b1->num[i]) - (b2->num[i]) - borrow;
        if(digit < 0)
        {
            borrow = 1;
            digit += 10;
        }
        else
        {
            borrow = 0;
        }
        diff->num[i] = digit;
        i++;
    }
    while(i < b1->length)
    {
        digit = (b1->num[i]) - borrow;
        if(digit < 0)
        {
            borrow = 1;
            digit += 10;
        }
        else
        {
            borrow = 0;
        }
        diff->num[i] = digit;
        i++;
    }
    diff->length = i;
    return diff;
}

// Subtraction
BigInt* subtractBigInts(BigInt *b1, BigInt *b2)
{
    int n1 = b1->length;
    int n2 = b2->length;
    int borrow;
    BigInt* diff = (BigInt*)malloc(sizeof(BigInt));
    if(n2 > n1)
        diff->num = (int*)malloc(sizeof(int)*n2);
    else
        diff->num = (int*)malloc(sizeof(int)*n1);
    
    // If the signed bits are not equal
    if(b1->signedBit != b2->signedBit)
    {
        if(b2->signedBit == 1)
        {
            b2->signedBit = 0;
            diff = addBigInts(b1, b2);
            b2->signedBit = 1;
            diff->signedBit = 0; 
        }    
        else
        {
            b2->signedBit = 1;
            diff = addBigInts(b1, b2);
            b2->signedBit = 0;
            diff->signedBit = 1; 
        }
    }
    else
    {
        if(compare(b1, b2) > 0)    // Magnitude of b1 > b2
        {
            diff = subtraction(b1, b2);    // Subtraction of magnitudes
            diff->signedBit = b1->signedBit;    // b1 - b2 gets sign of b1
        }
        else
        {
            diff = subtraction(b2, b1);
            if(b1->signedBit == 0)        // b1 - b2 and b1 < b2 --> diff has opposite sign of b1
                diff->signedBit = 1;
            else
                diff->signedBit = 0;
        }    
    }
    return diff;
}


BigInt* multBigInts(BigInt *b1, BigInt *b2);
BigInt* karatsubaMultiply(BigInt* x, BigInt* y);
BigInt* createBigInt(int length, int signedBit);
void freeBigInt(BigInt* b);
BigInt* shiftBigInt(BigInt* b, int shift);
BigInt* sliceBigInt(BigInt* b, int start, int end);



// Helper function to create a BigInt with given length
BigInt* createBigInt(int length, int signedBit) {
    BigInt* b = (BigInt*)malloc(sizeof(BigInt));
    b->num = (int*)calloc(length, sizeof(int));
    b->length = length;
    b->signedBit = signedBit;
    return b;
}

// Helper function to free a BigInt
void freeBigInt(BigInt* b) {
    if (b != NULL) {
        if (b->num != NULL) {
            free(b->num);
        }
        free(b);
    }
}

// Helper function to shift a BigInt left by 'shift' digits 
BigInt* shiftBigInt(BigInt* b, int shift) {
    BigInt* shifted = createBigInt(b->length + shift, b->signedBit);
    for (int i = 0; i < b->length; i++) {
        shifted->num[i + shift] = b->num[i];
    }
    return shifted;
}

// Helper function to get a slice of a BigInt from index 'start' to 'end'
BigInt* sliceBigInt(BigInt* b, int start, int end) {
    if (start >= end) {
        return createBigInt(0, 0);
    }
    BigInt* slice = createBigInt(end - start, 0);
    for (int i = start; i < end; i++) {
        slice->num[i - start] = b->num[i];
    }
    return slice;
}

// Standard multiplication (used as base case for Karatsuba)
BigInt* standardMultiply(BigInt *b1, BigInt *b2) {
    BigInt* mult = (BigInt*)malloc(sizeof(BigInt));
    mult->length = b1->length + b2->length;
    mult->num = (int*)malloc(sizeof(int)*(mult->length));
    int i, j, carry, prod;
    
    for(i = 0; i < mult->length; i++)    // Initializing
        mult->num[i] = 0;
        
    for(i = 0; i < b1->length; i++) {
        carry = 0;
        for(j = 0; j < b2->length; j++) {
            prod = (b1->num[i] * b2->num[j]) + carry;
            carry = prod / 10;
            mult->num[i + j] += (prod % 10);
            if(mult->num[i + j] > 9) {
                mult->num[i + j + 1] += 1;
                mult->num[i + j] %= 10;
            }
        }
        while(carry != 0) {
            mult->num[i + j] += (carry % 10);
            if(mult->num[i + j] > 9) {
                mult->num[i + j + 1] += 1;
                mult->num[i + j] %= 10;
            }
            carry /= 10;
            j++;
        }
    }
    
    if(b1->signedBit == b2->signedBit)
        mult->signedBit = 0;
    else
        mult->signedBit = 1;
    
    i = mult->length - 1;
    while(mult->num[i] == 0) {
        i--;
        (mult->length)--;
    }
    if(mult->length == 0) {   // If the input is 0, product is 0
        mult->signedBit = 0;  // If multiplied by negative number, - sign comes
        mult->length = 1;
    }
        
    return mult;
}

// Karatsuba multiplication algorithm
BigInt* karatsubaMultiply(BigInt* x, BigInt* y) {
    // Base case: if numbers are small, use standard multiplication
    if (x->length < 10 || y->length < 10) {
        return standardMultiply(x, y);
    }

    // Find the half point
    int m = (x->length > y->length) ? x->length / 2 : y->length / 2;

    // Split x into a (high) and b (low)
    BigInt* a = sliceBigInt(x, m, x->length);
    BigInt* b = sliceBigInt(x, 0, m);
    
    // Split y into c (high) and d (low)
    BigInt* c = sliceBigInt(y, m, y->length);
    BigInt* d = sliceBigInt(y, 0, m);

    // Compute ac = a * c
    BigInt* ac = karatsubaMultiply(a, c);
    
    // Compute bd = b * d
    BigInt* bd = karatsubaMultiply(b, d);
    
    // Compute (a+b)*(c+d)
    BigInt* a_plus_b = addBigInts(a, b);
    BigInt* c_plus_d = addBigInts(c, d);
    BigInt* ad_plus_bc = karatsubaMultiply(a_plus_b, c_plus_d);
    
    // Compute (ad + bc) = (a+b)(c+d) - ac - bd
    BigInt* temp = subtractBigInts(ad_plus_bc, ac);
    BigInt* ad_plus_bc_final = subtractBigInts(temp, bd);

    // Compute ac * 10^(2*m) + (ad + bc) * 10^m + bd
    BigInt* term1 = shiftBigInt(ac, 2 * m);
    BigInt* term2 = shiftBigInt(ad_plus_bc_final, m);
    BigInt* sum1 = addBigInts(term1, term2);
    BigInt* sum2 = addBigInts(sum1, bd);

    // Set the sign
    sum2->signedBit = (x->signedBit != y->signedBit) ? 1 : 0;

    // Clean up temporary variables
    freeBigInt(a);
    freeBigInt(b);
    freeBigInt(c);
    freeBigInt(d);
    freeBigInt(ac);
    freeBigInt(bd);
    freeBigInt(a_plus_b);
    freeBigInt(c_plus_d);
    freeBigInt(ad_plus_bc);
    freeBigInt(temp);
    freeBigInt(ad_plus_bc_final);
    freeBigInt(term1);
    freeBigInt(term2);
    freeBigInt(sum1);

    return sum2;
}

// Main multiplication function
BigInt* multBigInts(BigInt *b1, BigInt *b2) {
    // Handle zero cases
    if ((b1->length == 1 && b1->num[0] == 0) || (b2->length == 1 && b2->num[0] == 0)) {
        BigInt* zero = createBigInt(1, 0);
        zero->num[0] = 0;
        return zero;
    }
    
    // Make copies with equal length for easier processing
    int max_len = (b1->length > b2->length) ? b1->length : b2->length;
    BigInt* x = createBigInt(max_len, b1->signedBit);
    BigInt* y = createBigInt(max_len, b2->signedBit);
    
    // Copy numbers and pad with zeros if needed
    for (int i = 0; i < b1->length; i++) x->num[i] = b1->num[i];
    for (int i = 0; i < b2->length; i++) y->num[i] = b2->num[i];
    
    BigInt* result = karatsubaMultiply(x, y);
    
    // Clean up
    freeBigInt(x);
    freeBigInt(y);
    
    return result;
}

int main()
{
    BigInt num1, num2;
    char *n1, *n2;
    n1 = (char*)malloc(sizeof(char) * BIGINT_SIZE); // Dynamic allocation of the strings
    n2 = (char*)malloc(sizeof(char) * BIGINT_SIZE);
    
    printf("Enter number 1: ");
    scanf("%s", n1);
    printf("Enter number 2: ");
    scanf("%s", n2);
    
    num1 = stringToBigInt(n1);    // Converting string to BigInt
    num2 = stringToBigInt(n2);
    
    BigInt *ans = addBigInts(&num1, &num2);
    printf("SUM OF 2 NUMBERS: ");
    printBigInt(ans);
    
    ans = subtractBigInts(&num1, &num2);
    printf("\nDIFFERENCE OF 2 NUMBERS: ");
    printBigInt(ans);
    
    ans = multBigInts(&num1, &num2);
    printf("\nPRODUCT OF 2 NUMBERS: ");
    printBigInt(ans);
    
    return 0;
}
