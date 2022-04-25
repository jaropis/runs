# runs
Software for calculating all aspects of monotonic runs as used in the HRA field. The existing python and R implementations are not fast enough to process 48h recordings - hence this C++ software.

**Why the software is a bit weird?** i.e. why is there a mixture of old pointers to arrays and modern syntax like vectors? I would have preferred to write everything in a modern style, but I had requests from equipment companies to make it as C-frendly as possible. So, the parts which calculate runs are C-style, but the io stuff is modern. 