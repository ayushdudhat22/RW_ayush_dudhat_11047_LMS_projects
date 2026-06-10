const quizData = [
  {
    question: "1.What does HTML stand for?",
    options: [
      "Hyper Text Markup Language",
      "High Text Machine Language",
      "Home Tool Markup Language",
      "Hyper Tool Multi Language"
    ],
    answer: "Hyper Text Markup Language"
  },
  {
    question: "2.Which language is used for styling web pages?",
    options: ["HTML", "CSS", "Java", "Python"],
    answer: "CSS"
  },
  {
    question: "Which is not a JavaScript framework?",
    options: ["React", "Angular", "Vue", "Django"],
    answer: "Django"
  },
  {
    question: "4.Which tag is used to create a hyperlink in HTML?",
    options: ["link tag", "a tag", "href tag", "url tag"],
    answer: "a tag"
  },
  {
    question: "Which CSS property changes text color?",
    options: ["font-color", "text-color", "color", "background-color"],
    answer: "color"
  },
  {
    question: "6.How do you declare a variable in JavaScript?",
    options: ["var", "let", "const", "All of these"],
    answer: "All of these"
  },
  {
    question: "7.Which company developed JavaScript?",
    options: ["Microsoft", "Netscape", "Google", "Apple"],
    answer: "Netscape"
  },
  {
    question: "8.Which symbol is used for comments in JavaScript?",
    options: ["//", "<!-- -->", "#", "**"],
    answer: "//"
  },
  {
    question: "9.Which method displays data in the browser console?",
    options: ["print()", "console.log()", "display()", "write()"],
    answer: "console.log()"
  },
  {
    question: "10.Which HTML tag is used for the largest heading?",
    options: ["&lt;h6&gt;", "&lt;heading&gt;", "&lt;h1&gt;", "&lt;head&gt;"],
    answer: "&lt;h1&gt;"
  },
  {
    question: "11.Which CSS property is used to make text bold?",
    options: ["font-style", "font-weight", "text-bold", "bold"],
    answer: "font-weight"
  },
  {
    question: "12.Which JavaScript function converts a string to an integer?",
    options: ["parseInt()", "toString()", "Number()", "parseFloat()"],
    answer: "parseInt()"
  },
  {
    question: "13.Which operator is used for strict equality in JavaScript?",
    options: ["=", "==", "===", "!="],
    answer: "==="
  },
  {
    question: "14.Which HTML tag is used to insert an image?",
    options: ["&lt;picture&gt;", "&lt;image&gt;", "&lt;img&gt;", "&lt;src&gt;"],
    answer: "&lt;img&gt;"
  },
  {
    question: "15. Which event occurs when a button is clicked?",
    options: ["onhover", "onchange", "onclick", "onload"],
    answer: "onclick"
  }
];




let currentQuestion = 0;
let score = 0;

function loadQuestion() {
    const q = quizData[currentQuestion];

    document.getElementById("progress").innerText =
    `Question ${currentQuestion + 1} of ${quizData.length}`;

    document.getElementById("question").innerText = q.question;

    let optionsHTML = "";

    q.options.forEach(option => {
        optionsHTML += `
            <div class="option">
                  <label>
                        <input type="radio" name="answer" value="${option}">
                        ${option}
                  </label>
            </div>
        `;
    });

    document.getElementById("options").innerHTML = optionsHTML;
}

function nextQuestion() {
    const selected = document.querySelector(
        'input[name="answer"]:checked'
    );

    if (!selected) {
        alert("Please select an answer");
        return;
    }

    if (selected.value === quizData[currentQuestion].answer) {
        score++;
    }

    currentQuestion++;

    if (currentQuestion < quizData.length) {
        loadQuestion();
    } else {
         let percentage = ((score / quizData.length) * 100).toFixed(2);

    document.querySelector(".quiz-container").innerHTML = `
        <h2>Quiz Completed!</h2>
        <h3>Your Score: ${score}/${quizData.length}</h3>
        <h3>Percentage: ${percentage}%</h3>
        <button onclick="location.reload()">Restart Quiz</button>
    `;

    }
}

loadQuestion();