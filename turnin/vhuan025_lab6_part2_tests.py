# Array of tests to run (in order)
# Each test contains
#   description - 
#   steps - A list of steps to perform, each step can have
#       inputs - A list of tuples for the inputs to apply at that step
#       *time - The time (in ms) to wait before continuing to the next step 
#           and before checking expected values for this step. The time should be a multiple of
#           the period of the system
#       *iterations - The number of clock ticks to wait (periods)
#       expected - The expected value at the end of this step (after the "time" has elapsed.) 
#           If this value is incorrect the test will fail early before completing.
#       * only one of these should be used
#   expected - The expected output (as a list of tuples) at the end of this test
# An example set of tests is shown below. It is important to note that these tests are not "unit tests" in 
# that they are not ran in isolation but in the order shown and the state of the device is not reset or 
# altered in between executions (unless preconditions are used).

tests = [ {'description': 'wait 5 => PORTB: 0x04',
    'steps': [ {'inputs': [('PINA',0xFF)], 'iterations': 5 } ],
    'expected': [('PORTB',0x04)],
    },
    {'description': 'wait 1 and press => PORTB: 0x02',
    'steps': [ {'inputs': [('PINA',0xFF)], 'iterations': 1 },
               {'inputs': [('PINA',0xFE)], 'iterations': 2 },
               {'inputs': [('PINA',0xFF)], 'iterations': 2 }],
    'expected': [('PORTB',0x02)],
    },
    {'description': 'wait 2s, press, and press again, wait 1 => PORTB: 0x02',
    'steps': [ {'inputs': [('PINA',0xFF)], 'iterations': 2 },
               {'inputs': [('PINA',0xFE)], 'iterations': 2 },
               {'inputs': [('PINA',0xFF)], 'iterations': 2 },
               {'inputs': [('PINA',0xFE)], 'iterations': 2 },
               {'inputs': [('PINA',0xFF)], 'iterations': 2 }],
    'expected': [('PORTB',0x02)],
    },
    ]
#watch = ['PORTB']

