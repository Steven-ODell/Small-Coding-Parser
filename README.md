# Second attempt at a "Language"

## Planned syntax:

let x = 5;
let name = "Steven";
let passing = x > 3;

if (passing) {
    let bonus = x + 10;
    print(bonus);
} else {
    print(x);
}

fn add(a, b) {
    let result = a + b;
    return result;
}

let total = add(x, 10);
print(total);
