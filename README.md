# bit (WIP)
 Like Git, but mini.

A minimal solution to versioning without all the fancy features. 

Meant to be used as a form of saving and undoing recent changes. Do not use for repository management.

It also has no inbuilt functionality for nested files.

# Functionality

All commands.

----

Lists commands along with short description of each.
```
bit help
```

----

Pushes current changes to queue, removing the previous queue simultaniously. Note that you may only load commits that have been tagged.
```
bit add
```

----

Tags current changes in queue, allowing for jumping back to this change specifically.
```
bit tag
```

----

Runs add and tag consecutively.
```
bit at
```

----

Jump back to specific change. Note that if you jump back and 'add' consecutively, all would-be branches are removed. 
```
bit jump [relative index. ex: -2, -1, 1, 2, etc.]
```

----

View specific change. Lists files changed.
```
bit look
```

----

View history of changes. Lists files changed.
```
bit view
```

----

Clears out all history from this location and its children.
```
bit clear [default is all, if index set, then everything aside for the most recent to that index are saved]
```

----

Remove bit from location.
```
bit delete
```

----

Allows for changing the default command from `help` to another. 
```
bit default
```

# How it works

## Initialization

Occurs when the first add is ran and the `.bit` directory is generated.

Within the `.bit` directory the following are generated:
- `timeline` a file that holds the changes between the different versions in `bit`
- `settings` alters how bit interacts with your files.
- `/filestore` a directory to hold timeline files.

On initialization, the current files are tagged, hashed, and pushed autmatically to the timeline.

## Adding, Tagging, and Jumping

Adding files puts the current files you are editing into a queue. If you perform edits and perform the add function again, that queue is replaced with the files you most recently edited.

Tagging pushes your queue to the bit timeline. Allowing you to seek back to it and any previously tagged files.

Jumping allows the user to switch the current working files to those saved from the jumped-to version.

**Adding**:
- Generates a hash of each file.
- If the hash is unique relative to file name in `timeline`, it is kept in the queue.
  - Assigned name `[hash_of_file]`. Note that the filename and extension are not used in the hash. Instead they are used in the reference to this location in the timeline.
  - That hash is kept in `queuedata`.

**Tagging**:
- If the queue holds any files, those files are pushed into `filestore`.
- Once pushed to `filestore`, the hashes are saved into the timeline and tagged under their new (most recent) version.
- Each version keeps track of what files (via their file hash) they had.

**Jumping**:
- Checks each hash in the version being jumped to.
  - If the hash matches one in the current queue, it is ignored. 
  - If it does not match, it is pushed and the current files are changed to match.
  - If the hashs that don't match don't exist in that version, they are removed.

Timeline entry (and queue) format:
```
--- // entry seperator (not used in queue file)
[filename]::[filehash]
[filename]::[filehash]
[filename]::[filehash]
```



# Compilation

Compile
```
g++ src/main.cpp -o bin/bit.exe
```

Run
```
bit [command]
```

# TODO

## High Priority

- [ ] basic functionality
- [ ] commands
  - [ ] help
  - [ ] add
  - [ ] tag
  - [ ] at
  - [ ] jump
  - [ ] look
  - [ ] clear
  - [ ] default
- [ ] easier code readability
- [ ] full test suite

## Future Plans

- Ignore tags
  - Ignore specific files for pushing/pulling.
  - Allows for keepings, say, an application untouched while easily altering its save files.
- Nested file support and folder support
- UI
  - So more people can use the functions here
  - Compiled version is paid, code is completely open-source so people can compile it themselves if they want.
- Easier accessability for commands
  - Mainly so that we can a: call the function from anywhere, and b: so that more people actually use it. 
- Wiki
