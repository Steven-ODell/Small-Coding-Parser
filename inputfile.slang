fn square(n) {
    let result = n * n;
    return result;
}

fn add(a, b) {
    let result = a + b;
    return result;
}

fn subtract(a, b) {
    let result = a - b;
    return result;
}

fn multiply(a, b) {
    let result = a * b;
    return result;
}

fn divide(a, b) {
    let result = a / b;
    return result;
}

let x = 10;
let y = 4;
let z = 3;

let x_squared = square(x);
let y_squared = square(y);
let z_squared = square(z);

print(x_squared);
print(y_squared);
print(z_squared);

let sum_xy = add(x, y);
let sum_yz = add(y, z);
let sum_all = add(sum_xy, z);

print(sum_xy);
print(sum_yz);
print(sum_all);

let diff = subtract(x, y);
let product = multiply(x, y);
let quotient = divide(x, y);

print(diff);
print(product);
print(quotient);

let hyp = add(x_squared, y_squared);
print(hyp);

let check = x > y;
if (check) {
    let bonus = multiply(x, z);
    print(bonus);
} else {
    print(y);
}
