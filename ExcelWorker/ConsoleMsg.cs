using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ExcelWorker
{
    internal class ConsoleMsg
    {
        internal static void ErrorMessage(string message)
        {
            SetConsoleTextColor(ConsoleColor.Red); // red
            Console.Error.WriteLine(message);
            ResetConsoleTextColor();
        }

        internal static void WarningMessage(string message)
        {
            SetConsoleTextColor(ConsoleColor.Yellow); // yellow
            Console.Error.WriteLine(message);
            ResetConsoleTextColor();
        }

        internal static void SuccessMessage(string message)
        {
            SetConsoleTextColor(ConsoleColor.Green); // green
            Console.WriteLine(message);
            ResetConsoleTextColor();
        }

        internal static void InfoMessage(string message)
        {
            SetConsoleTextColor(ConsoleColor.Blue); // blue
            Console.WriteLine(message);
            ResetConsoleTextColor();
        }

        internal static void SetConsoleTextColor(ConsoleColor color)
        {
            Console.ForegroundColor = color;
        }

        internal static void ResetConsoleTextColor()
        {
            Console.ResetColor();
        }
    }
}
