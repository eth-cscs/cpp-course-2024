---
marp: true
theme: cscs
paginate: true
backgroundColor: #fff
backgroundImage: url('slides-data/common/4k-slide-bg-white.png')
size: 16:9
style: |
    div.hcenter {
        width: 60%;
        margin: 0 auto;
    }
    h1 {
        text-align: center;
    }
    img[alt~="center"] {
        display: block;
        margin: 0 auto;
    }
---
<!-- _paginate: skip  -->
<!-- _class: titlecover -->
<!-- _footer: "" -->

![bg cover](slides-data/common/title-bg3.png)

# **Concepts**
## An introduction

Alberto Invernizzi, CSCS (alberto.invernizzi@cscs.ch)

---
# Stroustrup about Templates

<div class="hcenter">

In about 1987, I tried to design templates with proper interfaces. I failed. **I wanted** three properties for templates:

- Full generality/expressiveness
- Zero overhead compared to hand coding
- **Well-specified interfaces**

Then, nobody could figure out how to get all three, so **we got**
- Turing completeness
- Better than hand-coding performance
- Lousy interfaces (basically **compile-time duck typing**)

**The lack of well-specified interfaces led to the spectacularly bad error messages we saw over the years.** The other two properties made templates a run-away success.

<span style="text-align:right; font-size: 23px">

from ["Concepts: The Future of Generic Programming"](https://wg21.link/p0557) in 2017

</span>

</div>

---
# What are concepts?

<div class="hcenter">

<center>

"Concepts are a type system for types"
(B. Milewski in [C++ Concepts: a Postmortem](https://bartoszmilewski.com/2010/06/24/c-concepts-a-postmortem/))

</center>

So they offer classic advantages of strongly-typed type systems:

- constrain types
- improve error reporting
- overloading
- self-documenting code

</div>

---
<!-- _class: lead -->
# Concepts

<div class="hcenter">
<cite>

“Concepts” is a foundational feature that in the ideal world would have been present in the very first version of templates and the basis for all use.

 (Bjarne Stroustrup in 2017)

</cite>
</div>

---
<!-- _class: lead -->

# Once upon a time...

---
<div class="hcenter">

It was early 2000s and `concept` concept was born.

...at least in C++ world, because it was not a completely new idea (e.g. Haskell already had `typeclass`es), but it needed some adaptions/extension for the C++ generic programming problems.

- In **2003** first `concept`s papers come up at WG21
(by Bjarne Stroustrup and Gabriel Dos Reis).

- In **2005** they were driven towards C++0x
with a paper [Concepts for C++0x](https://open-std.org/jtc1/sc22/wg21/docs/papers/2005/n1758.pdf)

- In **2006**, **2007**, **2008** more papers about `concept` and STL (e.g. https://www.stroustrup.com/popl06.pdf)
</div>

---
# Then 2009 came...

<div class="hcenter">

[WG21 July 2009 Meeting(13-18 July 2009)](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2009/n2920.html) happened, and there has been a discussion about what to do with concepts...

<center>

### TL;DR

</center>

![width:1000px](slides-data/attachments/sutter-wg21-july09-report.png)

which is the title of a [post](https://herbsutter.com/2009/07/21/trip-report/) from Herb Sutter where he interviewed Bjarne Stroustrup about the "exit" of `concept`s from the C++-0x standard.

</div>

---
<!-- _class: lead -->

<div class="hcenter" style="width:70%">

## <!--fit--> Extent of Stroustrup interview about Concepts C++-0x

**Q**: Were concepts removed because of political reason X, Y, or Z?

**A**: No. [...] The committee decided to **prioritize schedule and ship C++0x sooner** while still leaving the door wide open to also adopt concepts later.

</div>

---
<!-- _class: lead -->

# C++11
### no concepts as per decision in 2009

---
<!-- _class: lead -->

## From 2009 the work (started in 2003) continued...

Actually, from Bjarne Stroustrup words

<cite>

I started to design concepts from scratch in 2009.
In 2011, Alex Stepanov [...] attacked the problem from the user’s perspective: [...]. That re-booted the standards effort based on a new, fundamentally different, and better approach than the C++0x effort.
We now have a ISO TS (“Technical Specification”) for concepts.

</cite>

---
<!-- _class: lead -->

# C++14
### no concepts, but "Concept TS" is ongoing

---

<!-- _class: lead -->

# C++17
### it's time for concepts!!!
["Why I want Concepts, and why I want them sooner rather than later"](http://wg21.link/P0225)

---
<!-- _class: lead -->

# C++17
### ~~it's time for concepts!!!~~ no concepts

See [Why Concepts didn't make it to C++17](https://web.archive.org/web/20181002123639/http://honermann.net/blog/2016/03/06/why-concepts-didnt-make-cxx17/)

---
<!-- _class: lead -->

# 🍾 `concept`s made it in C++20! 🍾

---
# Compiler support

<center>

[cppreference.com C++20 compiler support](https://en.cppreference.com/w/cpp/compiler_support#cpp20)

</center>

![width:1400 center](slides-data/attachments/compiler-support-cpp20.png)

---
# Concepts-0x != Concepts

<div class="twocolumns">

<div>

**Stroustrup interview about Concepts C++-0x**

**Q**: Aren’t concepts about adding major new expressive power to the language, and so enable major new kinds of programs or programming styles?

**A**: Not really. **Concepts are almost entirely about getting better error messages**. [...]

Yes, **concepts would add one truly new expressive capability to the language, namely the ability to overload on concepts**. [...]

By far the most visible benefit of concepts lies in **clearer template error messages**, including the **ability to do separate checking** [...].

</div>

<div>

**C++ 20 Concepts**

- 🧐 concepts are almost entirely about getting better error messages.

- ✅ concepts would add one truly new expressive capability to the language, namely the ability to overload on concepts.

- ❌ ability to do separate checking

- ❌ no concept maps

</div>
</div>

---
<div class="hcenter">

# TODO

Recapping what can we do with Concepts

- constrain types
- overloading

<center>

**Can't we already do this without Concepts?!**

</center>

- SFINAE
- tag dispatching
- constexpr

<center>

</div>

---
<!-- _class: lead -->

# Listen to Bjarne Stroustrup!

<div class="hcenter">

<cite>

Try concepts!
They will dramatically improve your generic programming and make the current workarounds (e.g., traits classes) and low-level techniques (e.g., enable_if – based overloading) feel like error-prone and tedious assembly programming.

</cite>

</div>

---
<!-- _class: lead -->

<div class="hcenter">

Aim of this presentation is not to compare old and new.
The old way is still there and can be useful.

It is just to show how more expressive are concepts and how helpful they can be in producing more readable, straightforward and maintenable code.

</div>

---
<!-- _class: lead -->

# Concepts
(syntax and new language constructs)

---
# Concepts: Placeholder Declaration

<div class="hcenter">

As Milewsky said "concepts is a type-system for types".

Indeed they express something for a type; we can use concepts next to types to constrain them.

```cpp
std::floating_point T
```

Actually, it is a placeholder declaration (like `template <class T>`), which defines constraints over the provided/deducible placeholder.

In the case above it means _"the placeholder `T` is constrained to respect the concept `std::floating_point`"_.

**It seems a very trivial thing, but it is already super powerful and expressive.**

---

<div class="hcenter">

We can use it to declare a named placeholder (e.g. `<class T>`) or an anonymous placeholder (`auto`)

```cpp
template <std::floating_point T>
std::integral auto foo(T a, T b);

std::floating_point auto x = foo(13.26, 3.14);
```

... but we still have to follow their rules.

</div>

<div class="twocolumns">
<div>

<center>

❌ NOT VALID ❌

</center>

```cpp
int foo(std::floating_point U a);

template <class U>
int foo(std::floating_point U a);

template <class R>
std::integral R foo();

std::integral R foo();

std::signed_integral int x = foo(13.26, 3.14);
```

</div>
<div>

<center>

✅ VALID ✅

</center>

```cpp
template <std::integral R, std::floating_point T>
R foo(T a, T b);

template <std::integral R, std::floating_point U>
R foo(std::floating_point auto a, U b);

template <std::floating_point U>
std::integral auto foo(std::floating_point auto a, U b);

int foo(std::floating_point auto a,
        std::floating_point auto b);
```

</div>
</div>

---
<!-- _backgroundColor: lightblue -->

# QUIZ TIME

# What's the difference?

<div class="hcenter">

```cpp
void foo(std::floating_point auto, std::floating_point auto);
```

# vs

```cpp
template <std::floating_point T> void foo(T auto, T auto);
```

</div>

---
<!-- _backgroundColor: lightblue -->

# QUIZ TIME

# What's the difference?
<div class="hcenter">

```cpp
std::integral auto foo();
```

# vs

```cpp
template <std::floating_point R> R foo();
```

<!-- https://godbolt.org/z/vqehcPGdn -->

</div>

---
<!-- _class: lead -->

# [STL Concepts Library](https://en.cppreference.com/w/cpp/concepts)
### <span>en.cppreference.com/w/cpp/concepts</span>

#### (Core, Comparison, Object, Callable, Iterator, Algorithm, Ranges)
---
# Why is it so powerful?

<div class="hcenter">

Because when a constraint is not respected, the entire function is like it does not exist for that type. It is completely removed from the overload set.

# TODO

</div>

---
<!-- _backgroundColor: lightgreen -->

<div class="hcenter">

# TODO Write an overload set of functions that plays differently depending on type

</div>

---
<!-- _class: lead -->

# Before introducing new keywords...
There is still one thing worth mentioning that can be done without additional keywords

---
# Concepts: true/false checks

<div class="hcenter">

We used concepts for declaring a constrained placeholder, but actually we can use the same concept to check that an instantiated placeholder respects it.

```cpp
std::floating_point<T>
```

yields a `constexpr bool` value, similarly to the good old trait.

This means that we can test that a type respect a concept.

```cpp
static_assert(    std::floating_point<float>);
static_assert(not std::floating_point<int>);
```

which is semantically the same as

```cpp
static_assert(    std::is_floating_point_v<float>);
static_assert(not std::is_floating_point_v<int>);
```

</div>

---
<!-- _class: lead -->

# Let's introduce new keywords!

---
# `requires` clause

<div class="hcenter">

Till now we used concepts without using any new keyword.

Actually there are more ways to express the same constraint using the `requires` keyword.

**Constraining the template**

```cpp
template <class T> requires std::floating_point<T>
T foo(T a, T b) {}
```

Or **constraining the function**
```cpp
template <class T>
T foo(T a, T b) requires std::floating_point<T> {}
```

`requires` clause checks that the given `constexpr` is `true`.

</div>

---
<div class="hcenter">

This means that we can pass to a `require` clause a concept like we did, but also whatever expressions returning `constexpr bool`.

So, we can use a trait
```cpp
template <class T>
void foo() requires std::is_floating_point_v<T>;
```

but also a call to a `constexpr` function

```cpp
template <class T>
void foo(T&& x) requires (std::signbit(-10));
```

or compose a boolean expression

```cpp
template <class T>
requires (std::integral<T> && !std::same_as<char>)
void foo(T&& x);
```

**NOTE**: the constant expression must be a primary expression. It can be easily achieved by parenthesizing an expression.

</div>

---
<div class="hcenter">

Also an IILE (aka _"Immediately Invoked Lambda Expression"_) returning a `constexpr bool` is a valid constraint.

```cpp
#include <source_location>

template <class T>
requires (
    [](const auto& line) {
        return line % 2 == 0;
    }(std::source_location::current().line())
)
void foo(T&& x) {
    // foo implementation
}
```

<center>

https://godbolt.org/z/hnWM9PTcr

</center>

<center>

**TEASER**
It is not just about bragging, actually this has some clever usage...

</center>

</div>

---
<!-- _class: lead -->

# And after `requires` keyword...
# ...we can introduce `requires` keyword

---
# `requires` expression

<div class="hcenter">

It's the same keyword, but previously we were talking about `requires`-clause, now we are introducing `requires`-expression!

```cpp
requires (parameter-list) {
    requirement_1;
    requirement_2;
    ...
    requirement_n;
}
```

+ `parameter-list` like for functions (optional)
Useful to get "symbolic" entities for the sake of defining requirements.
+ Each requirement has to be **verified** in order for a requirement expression to be true

A `requires`-expression returns `true` if all requirements are **verified**, otherwise `false`.

</div>

---
# `requires requires`

<div class="hcenter">

+ `requires`-clause evaluates a boolean expression
+ `requires`-expression returns a boolean value

<center>

💡 Wait... I can combine them! 💡

</center>

```cpp
template <class T, class U>
requires requires {
    ...
    }
void foo(T a, U b) {
}
```

# **...and is it a good idea?**

---
<!-- _class: lead-->

# NO.

---
<!-- _class: lead -->

## `requires requires` is generally a code smell.
It might be better to define a concept for it instead of having something ad-hoc.


---
<!-- _class: lead -->

# For the moment we are forgiven.
Since we don't know (yet) how to define a custom named concept, we can do

<div class="hcenter">

```cpp
template <class T>
requires requires { req_1; req_2; ... }
void foo() {}
```

</div>

---
# `requires`-expression


<div class="twocolumns">

<div>

A `requires`-expression gives us the ability to express a very complex set of constraints.

```cpp
requires (parameter-list) {
    requirement_1;
    requirement_2;
    ...
    requirement_n;
}
```

and all of them must be **verified** at the same time, like if they were joined by an AND.

There are four types of requirements we can write in the body of the `requires`-expression, and for each one **VERIFIED** has a different meaning.

</div>
<div>

1️⃣ **SIMPLE**: does it build?

```cpp
a + b;
std::floating_point<T>;
```

2️⃣ **TYPE**: does it represent a type?

```cpp
typename A<B>;
typename B::type;
```

3️⃣ **COMPOUND**: does it build and return-type respects requirement?

```cpp
{ x + b } noexcept -> std::same_as<T>;
```

4️⃣ **NESTED**: does it evaluate true?

```cpp
requires std::floating_point<T>;
```

</div>
</div>

---
<!-- _backgroundColor: lightblue -->

# QUIZ TIME

<div class="hcenter">

```cpp
#include <iostream>
#include <concepts>

template <class T>
requires requires {
    std::floating_point<T>;
}
void bar() { std::cout << "FLOATING" << std::endl; }

template <class T>
void bar() { std::cout << "GENERIC" << std::endl; }

int main() {
    bar<float>();
    bar<int>();
}
```

```
FLOATING
FLOATING
```

<center>

https://godbolt.org/z/qWbadfjh1

</center>

</div>

---
# Requirement Verification

<div class="hcenter">

<center>

## VALID

</center>

After template arguments substitution, is it a valid expression for the language?

```cpp
std::floating_point<T>;
```

# vs

<center>

## TRUE

</center>

After template arguments substitution, is the expression `true`?

```cpp
requires std::floating_point<T>;
```

</div>

---
<!-- _backgroundColor: lightblue -->

# QUIZ TIME

<div class="hcenter">

In a compound requirement

```cpp
{expression} noexcept -> return-type-requirement;
```

`noexcept` and "return-type requirement" are optional

# What's the difference?

**compound requirement**

```cpp
{ expression };
```

**simple requirement**

```cpp
expression;
```

<center>

# <!--fit --> SOLUTION >>> https://timsong-cpp.github.io/cppwp/expr.prim.req.compound#2

</center>

</div>

---
# `requires`-expression parameter list

<cite>

A `requires`-expression can introduce local parameters using a parameter list. These parameters have no linkage, storage, or lifetime; they are only used as notation for the purpose of defining requirements.

</cite>

<div class="hcenter">


```cpp
requires (T a, T b) {
    { a == b } -> std::same_as<bool>;
};
```

</div>


Parameter list is somehow a shortand list of `declval`s you might want to use for requirement definition.

<div class="hcenter">

```cpp
requires {
    { declval<const T&>() == declval<const T&>() } -> std::same_as<bool>;
};
```

</div>

...and equivalently to `declval`, you can use them in unevaluated context (e.g. check type, check if builds) but not in evaluated context (because they don't actually exist).

</div>

---
<!-- _class: lead -->

# How to define a new custom named concept
and forget about about the horrendous `requires requires` construct

---
# `concept` keyword

<div class="hcenter">

Till now we used already defined concepts, all the ones already available in STL.

But we can define our ones!

```cpp
template <class>
concept concept_name = bool_expression;
```

This defines a **named concept** where `bool_expression` can be whatever returns a `constexpr bool` value:

- a boolean expression of concepts
- `type_trait`
- ...

<center>

... or a `requires`-expression!

</center>

</div>

---
<!-- _backgroundColor: lightblue -->

# QUIZ TIME

<div class="hcenter">

<center>

**What does this concept requires on X?**

</center>

```cpp
template <class X>
concept  = requires (X t) {
    []<class... Args>(const std::tuple<Args...>&) {}(t);
};
```

1. What type of requirement is used in the `requires`-expression?
_simple, type, compound, nested, ..._

2. What does it mean to be valid for that type of requirement?
   - "it builds"
"it is true"

3. What is it technically checking?
_"If I have `X t` and I pass it to the lambda function..."_

1. What does it implies?
_"If it builds, `X` has to be ..."_

</div>

---
<!-- _backgroundColor: lightgreen -->

# EXERCISE

### TODO define a concept (number? addable? ...)

Useful Resources:

- https://en.cppreference.com/w/cpp/concepts#Standard_library_concepts
- https://en.cppreference.com/w/cpp/iterator#Algorithm_concepts_and_utilities

---
<!-- _backgroundColor: lightgreen -->

# EXERCISE

<div class="hcenter">

**Invocable with tuple**:  Define a concept which checks if a function is callable with arguments contained in a tuple.

```cpp
void foo(const int&, float);

static_assert(    invocable_with_tuple_args<foo, std::tuple<int, float>>);
static_assert(not invocable_with_tuple_args<foo, std::tuple<int, std::string>>);
```

Try implementing it using
- `std::apply`
- `std::invocable`
- `std::convertible_from` on each argument


<center>

https://godbolt.org/z/E3rE4bcMq

**Look at the errors**

</center>

</div>

---
<!-- _class: lead -->
# Concepts and Readable Errors

<div class="hcenter">

Do concepts improve errors? **Yes and no.**

</div>

---

<div class="hcenter">

# <!-- fit --> Where do we start from? SFINAE errors

<center>

There's an error in the code below... 🧐

</center>

```cpp
#include <vector>
#include <algorithm>

struct Number {
    long long value_;
};

int main() {
    std::vector<int> integers;
    std::sort(integers.begin(), integers.end());

    std::vector<Number> numbers;
    std::sort(numbers.begin(), numbers.end());
}
```

</div>

---
<style scoped>
pre {
    color: white;
    white-space: pre-wrap;
    font-size: 5pt;
}
</style>

# GCC 13.2 - 79 lines of output

<!-- https://godbolt.org/z/34nq5K3rK -->

<pre style="color: black;">
In file included from /opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algobase.h:71,
                 from /opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/vector:62,
                 from <source>:1:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h: In instantiation of 'constexpr bool __gnu_cxx::__ops::_Iter_less_iter::operator()(_Iterator1, _Iterator2) const [with _Iterator1 = __gnu_cxx::__normal_iterator<Number*, std::vector<Number> >; _Iterator2 = __gnu_cxx::__normal_iterator<Number*, std::vector<Number> >]':
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1819:14:   required from 'void std::__insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1859:25:   required from 'void std::__final_insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1950:31:   required from 'void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:4861:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<Number*, vector<Number> >]'
<source>:13:14:   required from here
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:45:23: error: no match for 'operator<' (operand types are 'Number' and 'Number')
   45 |       { return *__it1 < *__it2; }
      |                ~~~~~~~^~~~~~~~
In file included from /opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algobase.h:67:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note: candidate: 'template<class _IteratorL, class _IteratorR, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_IteratorL, _Container>&, const __normal_iterator<_IteratorR, _Container>&)'
 1250 |     operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:45:23: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_IteratorL, _Container>'
   45 |       { return *__it1 < *__it2; }
      |                ~~~~~~~^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note: candidate: 'template<class _Iterator, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_Iterator, _Container>&, const __normal_iterator<_Iterator, _Container>&)'
 1258 |     operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:45:23: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_Iterator, _Container>'
   45 |       { return *__it1 < *__it2; }
      |                ~~~~~~~^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h: In instantiation of 'bool __gnu_cxx::__ops::_Val_less_iter::operator()(_Value&, _Iterator) const [with _Value = Number; _Iterator = __gnu_cxx::__normal_iterator<Number*, std::vector<Number> >]':
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1799:20:   required from 'void std::__unguarded_linear_insert(_RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Val_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1827:36:   required from 'void std::__insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1859:25:   required from 'void std::__final_insertion_sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1950:31:   required from 'void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:4861:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<Number*, vector<Number> >]'
<source>:13:14:   required from here
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:98:22: error: no match for 'operator<' (operand types are 'Number' and 'Number')
   98 |       { return __val < *__it; }
      |                ~~~~~~^~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note: candidate: 'template<class _IteratorL, class _IteratorR, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_IteratorL, _Container>&, const __normal_iterator<_IteratorR, _Container>&)'
 1250 |     operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:98:22: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_IteratorL, _Container>'
   98 |       { return __val < *__it; }
      |                ~~~~~~^~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note: candidate: 'template<class _Iterator, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_Iterator, _Container>&, const __normal_iterator<_Iterator, _Container>&)'
 1258 |     operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:98:22: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_Iterator, _Container>'
   98 |       { return __val < *__it; }
      |                ~~~~~~^~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h: In instantiation of 'bool __gnu_cxx::__ops::_Iter_less_val::operator()(_Iterator, _Value&) const [with _Iterator = __gnu_cxx::__normal_iterator<Number*, std::vector<Number> >; _Value = Number]':
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_heap.h:140:48:   required from 'void std::__push_heap(_RandomAccessIterator, _Distance, _Distance, _Tp, _Compare&) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Distance = long int; _Tp = Number; _Compare = __gnu_cxx::__ops::_Iter_less_val]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_heap.h:247:23:   required from 'void std::__adjust_heap(_RandomAccessIterator, _Distance, _Distance, _Tp, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Distance = long int; _Tp = Number; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_heap.h:356:22:   required from 'void std::__make_heap(_RandomAccessIterator, _RandomAccessIterator, _Compare&) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1635:23:   required from 'void std::__heap_select(_RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1910:25:   required from 'void std::__partial_sort(_RandomAccessIterator, _RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1926:27:   required from 'void std::__introsort_loop(_RandomAccessIterator, _RandomAccessIterator, _Size, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Size = long int; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:1947:25:   required from 'void std::__sort(_RandomAccessIterator, _RandomAccessIterator, _Compare) [with _RandomAccessIterator = __gnu_cxx::__normal_iterator<Number*, vector<Number> >; _Compare = __gnu_cxx::__ops::_Iter_less_iter]'
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_algo.h:4861:18:   required from 'void std::sort(_RAIter, _RAIter) [with _RAIter = __gnu_cxx::__normal_iterator<Number*, vector<Number> >]'
<source>:13:14:   required from here
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:69:22: error: no match for 'operator<' (operand types are 'Number' and 'Number')
   69 |       { return *__it < __val; }
      |                ~~~~~~^~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note: candidate: 'template<class _IteratorL, class _IteratorR, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_IteratorL, _Container>&, const __normal_iterator<_IteratorR, _Container>&)'
 1250 |     operator<(const __normal_iterator<_IteratorL, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1250:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:69:22: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_IteratorL, _Container>'
   69 |       { return *__it < __val; }
      |                ~~~~~~^~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note: candidate: 'template<class _Iterator, class _Container> bool __gnu_cxx::operator<(const __normal_iterator<_Iterator, _Container>&, const __normal_iterator<_Iterator, _Container>&)'
 1258 |     operator<(const __normal_iterator<_Iterator, _Container>& __lhs,
      |     ^~~~~~~~
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/stl_iterator.h:1258:5: note:   template argument deduction/substitution failed:
/opt/compiler-explorer/gcc-13.2.0/include/c++/13.2.0/bits/predefined_ops.h:69:22: note:   'Number' is not derived from 'const __gnu_cxx::__normal_iterator<_Iterator, _Container>'
   69 |       { return *__it < __val; }
      |                ~~~~~~^~~~~~~
Compiler returned: 1
</pre>

---
<style scoped>
pre {
    color: white;
    white-space: pre-wrap;
    font-size: 12pt;
}
</style>

# Clang 17.0.1 - (extent of) 9 errors, 200+ lines of output

<!-- https://godbolt.org/z/jYPszxnhz -->

<pre style="color: black;">
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/predefined_ops.h:69:22: error: invalid operands to binary expression ('Number' and 'Number')
   69 |       { return *__it < __val; }
      |                ~~~~~ ^ ~~~~~
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_heap.h:140:42: note: in instantiation of function template specialization '__gnu_cxx::__ops::_Iter_less_val::operator()<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, Number>' requested here
  140 |       while (__holeIndex > __topIndex && __comp(__first + __parent, __value))
      |                                          ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_heap.h:247:12: note: in instantiation of function template specialization 'std::__push_heap<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, long, Number, __gnu_cxx::__ops::_Iter_less_val>' requested here
  247 |       std::__push_heap(__first, __holeIndex, __topIndex,
      |            ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_heap.h:356:9: note: in instantiation of function template specialization 'std::__adjust_heap<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, long, Number, __gnu_cxx::__ops::_Iter_less_iter>' requested here
  356 |           std::__adjust_heap(__first, __parent, __len, _GLIBCXX_MOVE(__value),
      |                ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_algo.h:1635:12: note: in instantiation of function template specialization 'std::__make_heap<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, __gnu_cxx::__ops::_Iter_less_iter>' requested here
 1635 |       std::__make_heap(__first, __middle, __comp);
      |            ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_algo.h:1910:12: note: in instantiation of function template specialization 'std::__heap_select<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, __gnu_cxx::__ops::_Iter_less_iter>' requested here
 1910 |       std::__heap_select(__first, __middle, __last, __comp);
      |            ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_algo.h:1926:13: note: in instantiation of function template specialization 'std::__partial_sort<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, __gnu_cxx::__ops::_Iter_less_iter>' requested here
 1926 |               std::__partial_sort(__first, __last, __last, __comp);
      |                    ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_algo.h:1947:9: note: in instantiation of function template specialization 'std::__introsort_loop<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, long, __gnu_cxx::__ops::_Iter_less_iter>' requested here
 1947 |           std::__introsort_loop(__first, __last,
      |                ^
/opt/compiler-explorer/gcc-13.2.0/lib/gcc/x86_64-linux-gnu/13.2.0/../../../../include/c++/13.2.0/bits/stl_algo.h:4861:12: note: in instantiation of function template specialization 'std::__sort<__gnu_cxx::__normal_iterator<Number *, std::vector<Number>>, __gnu_cxx::__ops::_Iter_less_iter>' requested here
 4861 |       std::__sort(__first, __last, __gnu_cxx::__ops::__iter_less_iter());
      |            ^
</pre>

---

---
<!-- _class: lead -->

# Q&A

Alberto Invernizzi
Research Software Engineer @ CSCS

---








---
<!-- _class: lead -->
# BONUS

---
![bg](slides-data/attachments/quiz-question.jpg)

<h1 style="position: absolute; left: 0; top: 58%; text-align: center; color: white; width: 100%; font-size: 44pt">

How can you specify cv-qualified concepts?

</h1>

---
<!-- _class: lead -->
<style scoped>
    div.answer-block {
        position: absolute;
        bottom: 3%;
        width: 93%;

        background: white;

        display: grid;
        grid-template-columns: repeat(2, minmax(0, 1fr));
        gap: 1rem;
    }

    div > pre {
        text-align: center;
        font-size: 35pt;
        margin: 10px;
    }
</style>

![bg](slides-data/attachments/quiz-answer.jpg)

<div class="answer-block">

<div>

```cpp
const Concept auto & name
```

```cpp
const Concept auto * const name
```

```cpp
Concept const auto & name
```

</div>

<div>

```cpp
Concept auto const & name
```

```cpp
Concept auto & const name
```

```cpp
Concept auto * const res6 = &val;
```

</div>
</div>

---

![bg right:40%](slides-data/attachments/yoda.jpg)

# <!--fit--> How can you specify cv-qualified concepts?

<div class="twocolumns">

<div>

```cpp
✅ const Concept auto & name
```

```cpp
✅ const Concept auto * const name
```

```cpp
❌ Concept const auto & name
```

</div>
<div>

```cpp
✅ Concept auto const & name
```

```cpp
❌ Concept auto & const name
```

```cpp
✅ Concept auto * const res6
```

</div>

</div>

**HINTS**
- `const` applies to the full type
(type, or concept + placeholder)
- constraint helps defining the type, soconcept and placeholder should be considered as a single thing
(i.e. concept and placeholder cannot be split apart)

---
<!-- _class: lead -->

# Concepts vs Parameter Pack

---
# Concepts vs Parameter Pack

<div class="hcenter">

```cpp
#include <concepts>

template <class... Args>
concept AtLeast2 = requires sizeof...(Args) >= 2;

template <AtLeast2... Args>
void foo(Args&&...) {}

int main() {
    foo(1, 2);  // error: AtLeast2<int>
}
```

With the syntax
```cpp
template <Concept... Placeholder>
```
we're applying the type constraint to each single type of the parameter pack, **NOT** to the parameter pack as a whole.

</div>

---
# Concepts vs Parameter Pack

<div class="hcenter">

```cpp
#include <concepts>

template <class... Args>
concept AtLeast2 = requires sizeof...(Args) >= 2;

template <class... Args>
requires AtLeast2<Args...>
void foo(Args&&...) {}

int main() {
    // foo(1);     // error: as per requirement
    foo(1, 2);
}
```

Now we are requiring that the full parameter pack `Args...` respect the concept `AtLeast2`.

</div>
