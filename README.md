# jclisp

##Status -- not working. 

jclisp is a simple implementation of the Scheme dialect of LISP. So far, it operates mainly as a "calculator", in that it can only evaluate numerical expressions like the following:

```scheme
>> (+ 23.23 (* -666 13.2))
-8767.97
```

but soon will also handle pre- and self-defined functions:

```scheme
>> (define (square x) (* x x))
>> (/ 12.2 (square 23))
.02306
```

and basic language keywords like `if`, `map`, `for-each` and others.
