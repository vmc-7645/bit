# bit
 Like Git, but mini.

A minimal solution to versioning without all the fancy features. 

Meant to be used as a form of saving and undoing recent changes. Do not use for repository management.

# Functionality

All commands.


Lists commands along with short description of each.
```
bit help
```

Adds current changes history log. Note that you may only load commits that have been tagged.
```
bit add
```

Tags current changes in history, allowing for jumping back to this change specifically.
```
bit tag
```

Runs add and tag consecutively.
```
bit at
```

Jump back to specific change. Note that if you jump back and 'add' consecutively, all would-be branches are removed. 
```
bit jump [relative index. ex: -2, -1, 1, 2, etc.]
```

View history of changes. Lists files changed.
```
bit look [max viewable]
```

Clears out all history from this location and its children.
```
bit clear [default is all, if index set, then everything aside for the most recent to that index are saved]
```

Allows for changing the default command from `help` to another. 
```
bit default
```


# Compilation


```
g++ src/main.cpp -o bin/bit.exe
```

```
start bit.exe
```